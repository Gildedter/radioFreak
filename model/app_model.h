#ifndef APP_MODEL_H
#define APP_MODEL_H

#include <Arduino.h>
#include <WiFi.h>
#include "esp_wifi.h"
#include <SPI.h>
#include <RF24.h>
#include <radio_controller.h>
#include "config.h"

class AppModel {
public:
  AppModel();

  bool initRadio();
  void clearMonitorVals();

  int menuIndex() const { return _menuIndex; }
  void setMenuIndex(int index) { _menuIndex = index; }

  bool inScanner() const { return _inScanner; }
  void setInScanner(bool value) { _inScanner = value; }

  bool inMonitor() const { return _inMonitor; }
  void setInMonitor(bool value) { _inMonitor = value; }

  bool inInfo() const { return _inInfo; }
  void setInInfo(bool value) { _inInfo = value; }

  int ssidCount() const { return _ssidCount; }
  void setSsidCount(int count) { _ssidCount = count; }
  int ssidScroll() const { return _ssidScroll; }
  void setSsidScroll(int scroll) { _ssidScroll = scroll; }
  String ssidAt(int index) const { return _ssids[index]; }

  int monitorChannel() const { return _monitorChannel; }
  void setMonitorChannel(int channel) { _monitorChannel = channel; }

  unsigned long lastSampleMs() const { return _lastSampleMs; }
  void setLastSampleMs(unsigned long ms) { _lastSampleMs = ms; }

  unsigned long maxValue() const { return _maxValue; }
  double multiplier() const { return _multiplier; }
  uint32_t valAt(int index) const { return _vals[index]; }

  void resetMonitor();
  void startScan();
  void finishScan();
  void startMonitor();
  void stopMonitor();
  void sampleMonitor(unsigned long now);
  void onPromiscuousPacket(void* buf, wifi_promiscuous_pkt_type_t type);
  void jam();

private:
  int _menuIndex;

  bool _inScanner;
  bool _inMonitor;
  bool _inInfo;

  String _ssids[MAX_SSIDS];
  int _ssidCount;
  int _ssidScroll;

  uint32_t _vals[SAMPLE_WIDTH];
  int _monitorChannel;
  unsigned long _pktCount;
  unsigned long _deauthCount;
  unsigned long _lastSampleMs;
  unsigned long _maxValue;
  double _multiplier;

  RF24 _radio;
  byte _address[6];
  RadioController _rfController;
};

#endif
