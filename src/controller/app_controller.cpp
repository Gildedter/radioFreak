#include "controller/app_controller.h"
#include "config.h"

AppController::AppController()
  : _lastBtnMs(0),
    _lastBtnState(0) {}

void AppController::setup() {
  Serial.begin(115200);
  delay(50);

  _view.begin();

  pinMode(ANALOG_PIN, INPUT);
  _model.clearMonitorVals();

  // safe RF24 init (do not transmit)
  if (!_model.initRadio()) {
    Serial.println("Radio init failed!");
    while (true);
  }
  Serial.println("Setup complete.");

  _view.drawMainMenu(_model);
  Serial.println("Ready. Use buttons (ladder on D2).");
}

void AppController::loop() {
  int btn = readButtonsDebounced();

  if (!_model.inScanner() && !_model.inMonitor() && !_model.inInfo()) {
    if (btn == 1) {
      _model.setMenuIndex((_model.menuIndex() == 0) ? MENU_COUNT - 1 : _model.menuIndex() - 1);
      _view.drawMainMenu(_model);
    }
    if (btn == 3) {
      _model.setMenuIndex((_model.menuIndex() + 1) % MENU_COUNT);
      _view.drawMainMenu(_model);
    }
    if (btn == 2) { // SELECT
      switch (_model.menuIndex()) {
        case 0:
          enterScanner();
          break;
        case 1:
          enterMonitor();
          break;
        case 2:
          _view.drawDeauthPlaceholder();
          delay(800);
          _view.drawMainMenu(_model);
          break;
        case 3:
          enterInfo();
          break;
      }
    }
  } else if (_model.inScanner()) {
    if (btn == 1 && _model.ssidScroll() > 0) {
      _model.setSsidScroll(_model.ssidScroll() - 1);
      _view.drawScanner(_model);
    }
    if (btn == 3 && _model.ssidScroll() + 5 < _model.ssidCount()) {
      _model.setSsidScroll(_model.ssidScroll() + 1);
      _view.drawScanner(_model);
    }
    if (btn == 2) {
      exitScanner();
      _view.drawMainMenu(_model);
    }
  } else if (_model.inInfo()) {
    if (btn == 2) {
      exitInfo();
      _view.drawMainMenu(_model);
    }
  }

  delay(10);
}

void AppController::onWifiPacket(void* buf, wifi_promiscuous_pkt_type_t type) {
  _model.onPromiscuousPacket(buf, type);
}

int AppController::analogReadAvg(int pin, int samples) {
  long sum = 0;
  for (int i = 0; i < samples; i++) {
    sum += analogRead(pin);
    delay(3);
  }
  return int(sum / samples);
}

int AppController::readButtonsDebounced() {
  int raw = analogReadAvg(ANALOG_PIN, 6);
  int btn = 0;
  if (raw >= BTN_UP_MIN && raw <= BTN_UP_MAX) btn = 1;
  else if (raw >= BTN_SELECT_MIN && raw <= BTN_SELECT_MAX) btn = 2;
  else if (raw >= BTN_DOWN_MIN && raw <= BTN_DOWN_MAX) btn = 3;
  else btn = 0;

  unsigned long now = millis();
  if (btn != _lastBtnState) {
    _lastBtnMs = now;
    _lastBtnState = btn;
    return 0;
  } else {
    if (btn != 0 && (now - _lastBtnMs) > DEBOUNCE_MS) {
      _lastBtnState = -1;
      _lastBtnMs = now;
      return btn;
    }
    if (btn == 0 && _lastBtnState == -1) _lastBtnState = 0;
  }
  return 0;
}

void AppController::enterScanner() {
  _model.setInScanner(true);
  _model.setSsidScroll(0);
  _model.setSsidCount(0);

  _view.drawScanning();
  _model.startScan();
  _view.drawScanner(_model);
}

void AppController::exitScanner() {
  _model.setInScanner(false);
  _model.finishScan();
}

void AppController::enterMonitor() {
  _model.setInMonitor(true);
  _model.setLastSampleMs(millis());
  _model.resetMonitor();

  _model.startMonitor();

  _view.drawMonitorSplash();
  delay(800);

  while (_model.inMonitor()) {
    int btn = readButtonsDebounced();
    if (btn == 1 && _model.monitorChannel() < CHANNEL_MAX) {
      _model.setMonitorChannel(_model.monitorChannel() + 1);
      esp_wifi_set_channel(_model.monitorChannel(), WIFI_SECOND_CHAN_NONE);
      _model.resetMonitor();
      delay(150);
    } else if (btn == 3 && _model.monitorChannel() > 1) {
      _model.setMonitorChannel(_model.monitorChannel() - 1);
      esp_wifi_set_channel(_model.monitorChannel(), WIFI_SECOND_CHAN_NONE);
      _model.resetMonitor();
      delay(150);
    } else if (btn == 2) {
      exitMonitor();
      _view.drawMainMenu(_model);
      break;
    }

    unsigned long now = millis();
    if (now - _model.lastSampleMs() >= SAMPLE_INTERVAL_MS) {
      _model.sampleMonitor(now);
      _view.drawMonitor(_model);
    }
    delay(8);
  }
}

void AppController::exitMonitor() {
  _model.stopMonitor();
  _model.setInMonitor(false);
}

void AppController::enterInfo() {
  _model.setInInfo(true);
  _view.drawInfoScreen();
  _model.jam();
}

void AppController::exitInfo() {
  _model.setInInfo(false);
}
