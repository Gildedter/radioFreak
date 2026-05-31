# Bandit (radioFreak)

Multi-purpose ESP32 handheld tool with an nRF24L01 module and SSD1306 OLED. Provides a button-driven menu for WiFi scanning, packet monitoring, and 2.4 GHz RF control.

## Features

| Menu item | Description |
|-----------|-------------|
| **Scan** | WiFi network scan with scrollable SSID list |
| **Packet_Monitor** | Promiscuous-mode capture on channels 1–11 with live packet graph |
| **Deauth** | Placeholder screen (no transmit) |
| **JAM** | Info screen, then nRF24L01 constant-carrier channel hopping |

## Hardware

| Component | Connection |
|-----------|------------|
| ESP32 | Main MCU |
| SSD1306 OLED (128×64) | I2C — SDA GPIO 21, SCL GPIO 22, address `0x3C` |
| Analog button ladder | GPIO 2 (Up / Select / Down via voltage dividers) |
| nRF24L01 | SPI — CE GPIO 4, CSN GPIO 5 |

## Project structure

The firmware uses an embedded **Model–View–Controller** layout:

```
radioFreak/
├── radioFreak.ino          # Entry point: setup(), loop(), WiFi callback trampoline
├── config.h                # Pins, timing, and menu constants
├── radio_controller.h      # nRF24L01 hardware wrapper
├── model/
│   └── app_model.h         # Application state, WiFi scan/monitor, RF jam logic
├── view/
│   └── display_view.h      # OLED rendering
├── controller/
│   └── app_controller.h    # Button input and mode orchestration
└── src/
    ├── config.cpp
    ├── model/app_model.cpp
    ├── view/display_view.cpp
    └── controller/app_controller.cpp
```

- **Model** — Holds state and domain logic (WiFi, promiscuous capture, nRF24L01).
- **View** — Draws all screens to the OLED; reads model state only.
- **Controller** — Reads buttons, coordinates model and view, runs the main loop.

Implementation files live under `src/` so the Arduino build system picks them up automatically.

## Dependencies

Install via the Arduino Library Manager:

- [Adafruit GFX Library](https://github.com/adafruit/Adafruit-GFX-Library)
- [Adafruit SSD1306](https://github.com/adafruit/Adafruit_SSD1306)
- [RF24](https://github.com/nRF24/RF24)

Board package: **esp32** by Espressif Systems (tested with 3.3.x).

## Build and flash

1. Open the `radioFreak` folder in Arduino IDE 2.x.
2. Select board **ESP32 Dev Module** (or your specific ESP32 variant).
3. Install the libraries listed above if prompted.
4. Compile and upload.

The sketch file must be named `radioFreak.ino` to match the folder name.

## License

MIT — see [LICENSE](LICENSE).
