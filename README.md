A WiFi portal for microcontroller especially esp8266
## Install

Clone this project

```bash
  git clone https://github.com/Jars44/wifi-portal.git
```
=======
# WiFi Portal for ESP8266

This project is a WiFi portal designed specifically for microcontrollers, especially the ESP8266 (NodeMCU v2 board). It creates a captive portal that allows users to connect to a WiFi access point and log in through a modern web interface.

## Features

- Sets up a WiFi Soft Access Point (SSID: "XI RPL 3") without a password.
- Runs a DNS server and asynchronous web server to serve the captive portal.
- Provides a stylish login page with light/dark mode toggle.
- Plays a sound upon successful login.
- Tracks the number of logged-in users and manages user timeout.
- Uses an onboard LED (pin D2) to indicate user presence:
  - LED off: no users connected
  - LED steady on: 1 user connected
  - LED blinking faster: multiple users connected
- Uses LittleFS filesystem to serve static files like sound.

## Prerequisites

- Hardware: ESP8266 NodeMCU v2 or compatible board.
- Software: [PlatformIO](https://platformio.org/) IDE or CLI installed.
- USB cable to connect the ESP8266 board to your computer.

## Installation and Setup

1. Clone this repository:

```bash
git clone https://github.com/Jars44/wifi-portal.git
cd wifi-portal
```

2. Open the project in PlatformIO IDE or use PlatformIO CLI.

3. Build and upload the firmware to your ESP8266 board:

```bash
platformio run --target upload
```

4. Open the serial monitor to view debug output:

```bash
platformio device monitor
```

## Usage

1. After uploading, the ESP8266 will create a WiFi access point named **XI RPL 3**.

2. Connect your device (phone, laptop, etc.) to this WiFi network.

3. Open a web browser; you will be redirected to the login page automatically.

4. The login page features a button to log in and toggle between light and dark modes.

5. Press the **Login** button to log in. A sound will play to confirm login success.

6. The onboard LED indicates the number of connected users as described above.

## Project Structure

- `src/main.cpp`: Main firmware code handling WiFi, web server, DNS, and user logic.
- `platformio.ini`: Project configuration including board, framework, and dependencies.
- `data/sound.mp3`: Sound file served on login.
- `include/`, `lib/`, `test/`: Additional project folders with relevant code and tests.

## Dependencies

- [ESP Async WebServer](https://github.com/me-no-dev/ESPAsyncWebServer)
- [ESPAsyncTCP](https://github.com/me-no-dev/ESPAsyncTCP)
- LittleFS filesystem for serving static files.

## License

This project is licensed under the terms of the MIT License. See the [LICENSE](LICENSE) file for details.
