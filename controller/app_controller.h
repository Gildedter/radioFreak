#ifndef APP_CONTROLLER_H
#define APP_CONTROLLER_H

#include <Arduino.h>
#include "esp_wifi.h"
#include "model/app_model.h"
#include "view/display_view.h"

class AppController {
public:
  AppController();

  void setup();
  void loop();
  void onWifiPacket(void* buf, wifi_promiscuous_pkt_type_t type);

private:
  int analogReadAvg(int pin, int samples = 6);
  int readButtonsDebounced();

  void enterScanner();
  void exitScanner();
  void enterMonitor();
  void exitMonitor();
  void enterInfo();
  void exitInfo();

  AppModel _model;
  DisplayView _view;

  unsigned long _lastBtnMs;
  int _lastBtnState;
};

#endif
