#ifndef DISPLAY_VIEW_H
#define DISPLAY_VIEW_H

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "config.h"

class AppModel;

class DisplayView {
public:
  DisplayView();

  bool begin();
  void drawMainMenu(const AppModel& model);
  void drawScanning();
  void drawScanner(const AppModel& model);
  void drawDeauthPlaceholder();
  void drawInfoScreen();
  void drawMonitorSplash();
  void drawMonitor(const AppModel& model);

private:
  void drawSkull(int x, int y);

  Adafruit_SSD1306 _display;
};

#endif
