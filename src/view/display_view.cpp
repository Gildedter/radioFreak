#include "view/display_view.h"
#include "model/app_model.h"

const unsigned char skull_bitmap[] PROGMEM = {
  0b00001111, 0b11110000,
  0b00011111, 0b11111000,
  0b00111111, 0b11111100,
  0b01111000, 0b00011110,
  0b01110000, 0b00001110,
  0b11100110, 0b01100111,
  0b11100110, 0b01100111,
  0b11100111, 0b11100111,
  0b11100111, 0b11100111,
  0b11100110, 0b01100111,
  0b01110000, 0b00001110,
  0b01111000, 0b00011110,
  0b00111111, 0b11111100,
  0b00011111, 0b11111000,
  0b00001111, 0b11110000,
  0b00000000, 0b00000000
};

DisplayView::DisplayView()
  : _display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET) {}

bool DisplayView::begin() {
  Wire.begin(I2C_SDA, I2C_SCL);
  if (!_display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    Serial.println("SSD1306 init failed");
    while (1) delay(500);
  }
  _display.clearDisplay();
  _display.setTextSize(1);
  _display.setTextColor(SSD1306_WHITE);
  return true;
}

void DisplayView::drawMainMenu(const AppModel& model) {
  _display.clearDisplay();
  _display.setTextSize(1);
  _display.setTextColor(SSD1306_WHITE);

  // display.drawFastHLine(0, 0, 128, SSD1306_WHITE);
  // display.drawFastHLine(0, 20, 128, SSD1306_WHITE);
  // display.drawFastHLine(0, 40, 128, SSD1306_WHITE);
  // display.drawFastHLine(0, 60, 128, SSD1306_WHITE);

  for (int i = 0; i < MENU_COUNT; i++) {
    int y = ROW_Y[i];
    if (i == model.menuIndex()) {
      // display.fillRect(0, y-2, 128, 14, SSD1306_WHITE);
      const int HIGHLIGHT_H = 12;
      int yRect = max(0, y - 2);
      _display.fillRect(0, yRect, 128, HIGHLIGHT_H, SSD1306_WHITE);
      _display.setTextColor(SSD1306_BLACK);
    } else {
      _display.setTextColor(SSD1306_WHITE);
    }
    _display.setCursor(4, y);
    _display.print(menu_options[i]);
    _display.setTextColor(SSD1306_WHITE);
  }
  _display.display();
}

void DisplayView::drawScanning() {
  _display.clearDisplay();
  _display.setCursor(0, 0);
  _display.println("Scanning networks...");
  _display.display();
}

void DisplayView::drawScanner(const AppModel& model) {
  _display.clearDisplay();
  _display.setTextSize(1);
  _display.setCursor(0, 0);
  _display.println("WiFi Scanner");
  _display.drawFastHLine(0, 12, 128, SSD1306_WHITE);

  int perPage = 5;
  for (int i = 0; i < perPage; i++) {
    int idx = model.ssidScroll() + i;
    int y = 16 + i * 10;
    _display.setCursor(0, y);
    if (idx < model.ssidCount()) {
      String s = model.ssidAt(idx);
      if (s.length() > 18) s = s.substring(0, 18);
      _display.print(idx + 1);
      _display.print(". ");
      _display.print(s);
    } else {
      _display.print("");
    }
  }
  _display.display();
}

void DisplayView::drawDeauthPlaceholder() {
  _display.clearDisplay();
  _display.setCursor(0, 0);
  _display.println("Deauth module");
  _display.println("(placeholder - no transmit)");
  _display.display();
}

void DisplayView::drawInfoScreen() {
  _display.clearDisplay();
  _display.setTextSize(1);
  _display.setTextColor(SSD1306_WHITE);
  _display.setCursor(0, 0);
  //display.println("");
  _display.println("--------------");
  _display.println("2.4GHZ Jammer");
  _display.println("Elijah Cyber");
  _display.println("Version 1.0");
  drawSkull(96, 40); // small skull bottom-right
  _display.display();
}

void DisplayView::drawMonitorSplash() {
  _display.clearDisplay();
  _display.setTextSize(1);
  _display.setCursor(0, 0);
  _display.println("Packet-Monitor");
  _display.setCursor(0, 16);
  _display.println("Elijah Cyber");
  _display.display();
}

void DisplayView::drawMonitor(const AppModel& model) {
  _display.clearDisplay();
  _display.setTextSize(1);
  _display.setCursor(0, 0);
  _display.print("Packet-Monitor  Ch:");
  _display.print(model.monitorChannel());
  _display.setCursor(0, 10);
  _display.print("Pkts/interval:");
  _display.print(model.valAt(SAMPLE_WIDTH - 1));
  _display.setCursor(80, 0);
  _display.print("Total:");
  _display.print(model.maxValue());

  for (int i = 0; i < SAMPLE_WIDTH; i++) {
    double scaled = model.valAt(i) * model.multiplier();
    int h = (int)scaled;
    if (h > 48) h = 48;
    _display.drawFastVLine(i, 63 - h, h, SSD1306_WHITE);
  }
  _display.display();
}

void DisplayView::drawSkull(int x, int y) {
  _display.drawBitmap(x, y, skull_bitmap, 16, 16, SSD1306_WHITE);
}
