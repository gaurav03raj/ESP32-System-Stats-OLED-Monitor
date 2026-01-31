# ESP32-System-Stats-OLED-Monitor

# ESP32 System Stats OLED Monitor

A lightweight, real-time hardware monitor that displays PC performance metrics (CPU, RAM, and GPU) on an external I2C OLED screen via an ESP32.

---

## Project Description
This project uses a **Python client** to fetch system telemetry and sends it over a **Serial (USB) connection** to an **ESP32**. The ESP32 then renders these stats onto a 128x64 SSD1306 OLED display. It’s the perfect desk accessory for monitoring your PC's health without taking up screen real estate.

---

## Customizable Parameters

You can easily tweak the following variables in the source code to match your specific hardware or preferences:

### 1. ESP32 Firmware (`.ino` file)
* **`SCREEN_WIDTH` / `SCREEN_HEIGHT`**: Default is `128`x`64`. Change to `128`x`32` if using the smaller OLED strip.
* **`OLED_RESET`**: Set to `-1` if your module doesn't have a reset pin.
* **`SCREEN_ADDRESS`**: Usually `0x3C`, but some displays use `0x3D`.
* **`BAUD_RATE`**: Default is `115200`. (Must match the Python script).

### 2. Python Client (`.py` file)
* **`SERIAL_PORT`**: Update this to match your ESP32 (e.g., `COM3` on Windows or `/dev/ttyUSB0` on Linux).
* **`UPDATE_INTERVAL`**: Set the delay (in seconds) between data refreshes. (Default: `1`).
* **`METRICS`**: Toggle which data points to send (CPU load, Temperature, RAM usage, or GPU load).

---

## Quick Setup
1. **Hardware**: Connect ESP32 `3.3V` -> OLED `VCC`, `GND` -> `GND`, `GPIO 21` -> `SDA`, and `GPIO 22` -> `SCL`.
2. **Firmware**: Upload the sketch using Arduino IDE (requires `Adafruit_SSD1306` library).
3. **Software**: Install Python dependencies: `pip install psutil pyserial`.
4. **Run**: Start the Python script and watch your stats come to life!

---

## Credits
Created by:  
[![EasyCircuits YouTube](https://img.shields.io/badge/YouTube-EasyCircuits-red?style=for-the-badge&logo=youtube)](https://www.youtube.com/@EasyCircuits)

If you found this project helpful, consider subscribing for more hardware and coding tutorials!
