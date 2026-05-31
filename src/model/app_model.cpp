#include "model/app_model.h"

extern void wifiPromiscuousCb(void* buf, wifi_promiscuous_pkt_type_t type);

AppModel::AppModel()
  : _menuIndex(0),
    _inScanner(false),
    _inMonitor(false),
    _inInfo(false),
    _ssidCount(0),
    _ssidScroll(0),
    _monitorChannel(1),
    _pktCount(0),
    _deauthCount(0),
    _lastSampleMs(0),
    _maxValue(1),
    _multiplier(1.0),
    _radio(CE_PIN, CSN_PIN),
    _rfController(_radio, _address) {
  memcpy(_address, "node1", 6);
}

bool AppModel::initRadio() {
  if (!_rfController.begin()) {
    return false;
  }
  _rfController.configureTX();
  return true;
}

void AppModel::clearMonitorVals() {
  memset(_vals, 0, sizeof(_vals));
}

void AppModel::resetMonitor() {
  memset(_vals, 0, sizeof(_vals));
  _pktCount = 0;
  _deauthCount = 0;
  _maxValue = 1;
  _multiplier = 1.0;
}

void AppModel::startScan() {
  _ssidScroll = 0;
  _ssidCount = 0;

  WiFi.mode(WIFI_MODE_STA);
  WiFi.disconnect();
  delay(120);
  int n = WiFi.scanNetworks();
  _ssidCount = min(n, MAX_SSIDS);
  for (int i = 0; i < _ssidCount; i++) {
    _ssids[i] = WiFi.SSID(i);
  }
}

void AppModel::finishScan() {
  WiFi.scanDelete();
}

void AppModel::startMonitor() {
  WiFi.mode(WIFI_MODE_STA);
  esp_wifi_set_promiscuous_rx_cb(&wifiPromiscuousCb);
  esp_wifi_set_promiscuous(true);
  esp_wifi_set_channel(_monitorChannel, WIFI_SECOND_CHAN_NONE);
}

void AppModel::stopMonitor() {
  esp_wifi_set_promiscuous(false);
  esp_wifi_set_promiscuous_rx_cb(NULL);
}

void AppModel::sampleMonitor(unsigned long now) {
  _lastSampleMs = now;
  memmove(_vals, _vals + 1, (SAMPLE_WIDTH - 1) * sizeof(_vals[0]));
  _vals[SAMPLE_WIDTH - 1] = _pktCount;
  if (_pktCount > _maxValue) {
    _maxValue = _pktCount;
  }
  _multiplier = _maxValue > 47 ? 47.0 / (double)_maxValue : 1.0;
  _pktCount = 0;
}

void AppModel::onPromiscuousPacket(void* buf, wifi_promiscuous_pkt_type_t type) {
  if (!_inMonitor) return;
  wifi_promiscuous_pkt_t* pkt = (wifi_promiscuous_pkt_t*)buf;
  _pktCount++;
  if (pkt->rx_ctrl.sig_len > 12) {
    uint8_t ctl = pkt->payload[12];
    if (ctl == 0xA0 || ctl == 0xC0) _deauthCount++;
  }
}

void AppModel::jam() {
  _rfController.startCarrier(RF24_PA_MAX, 45);
  while (true) {
    uint8_t newChannel = random(0, BLUETOOTH_CHANNEL_COUNT);
    byte channel = bluetooth_channels[newChannel];
    _rfController.setChannel(channel);
  }
}
