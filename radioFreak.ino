/*
  Fixed ESP32 sketch (safe version)
  - SSD1306 (I2C 0x3C) on SDA=21, SCL=22
  - Analog button ladder on D2 -> GPIO2
  - Menu: Scan / Packet Monitor / Deauth / Info
  - Scanner: scrollable SSID list
  - Packet monitor: promiscuous capture by channel with graph
  - Info screen: skull bitmap
  - SAFE: Radio initialized but no carrier or jamming functionality included
*/

#include "controller/app_controller.h"

AppController app;

void setup() {
  app.setup();
}

void loop() {
  app.loop();
}

void wifiPromiscuousCb(void* buf, wifi_promiscuous_pkt_type_t type) {
  app.onWifiPacket(buf, type);
}
