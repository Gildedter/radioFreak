#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

// ---------------- Display ----------------
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define I2C_SDA 21
#define I2C_SCL 22
#define OLED_ADDR 0x3C

// ---------------- Buttons (analog ladder on D2) ----------------
const int ANALOG_PIN = 2;
const int BTN_UP_MIN     = 950;
const int BTN_UP_MAX     = 1250;
const int BTN_SELECT_MIN = 300;
const int BTN_SELECT_MAX = 550;
const int BTN_DOWN_MIN   = 1600;
const int BTN_DOWN_MAX   = 1950;
const unsigned long DEBOUNCE_MS = 120;

// ---------------- Menu ----------------
extern const char* menu_options[];
const int MENU_COUNT = 4;
extern const int ROW_Y[4]; // fit inside 0..63

// ---------------- Scanner ----------------
const int MAX_SSIDS = 60;

// ---------------- Packet monitor ----------------
const int SAMPLE_WIDTH = 128;
const int CHANNEL_MAX = 11;
const unsigned long SAMPLE_INTERVAL_MS = 500;

// ---------------- RF24 ----------------
#define CE_PIN  4
#define CSN_PIN 5
const int BLUETOOTH_CHANNEL_COUNT = 21;
extern const byte bluetooth_channels[BLUETOOTH_CHANNEL_COUNT];

#endif
