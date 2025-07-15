# WiFi Portal for ESP8266

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)
[![PlatformIO](https://img.shields.io/badge/PlatformIO-ESP8266-blue)](https://platformio.org/)

This project is a WiFi portal designed specifically for microcontrollers, especially the ESP8266 (NodeMCU v2 board). It creates a captive portal that allows users to connect to a WiFi access point and log in through a modern web interface.

## Features

- Sets up a WiFi Soft Access Point (SSID: "XI RPL 3") without a password by default.
- Runs a DNS server and asynchronous web server to serve the captive portal.
- Provides a stylish login page with light/dark mode toggle.
- Plays a sound upon successful login.
- Tracks the number of logged-in users and manages user timeout.
- Uses an onboard LED (pin D2) to indicate user presence:
  - LED off: no users connected
  - LED steady on: 1 user connected
  - LED blinking faster: multiple users connected
- Serves the login page and sound file from embedded HTML and LittleFS filesystem.

## Prerequisites

- Hardware: ESP8266 NodeMCU v2 or compatible board.
- Software: [PlatformIO](https://platformio.org/) IDE or CLI installed.
- USB cable to connect the ESP8266 board to your computer.

## Getting Started

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

1. After uploading, the ESP8266 will create a WiFi access point named **XI RPL 3** by default.

2. Connect your device (phone, laptop, etc.) to this WiFi network.

3. Open a web browser; you will be redirected to the login page automatically.

4. The login page features a button to log in and toggle between light and dark modes.

5. Press the **Login** button to log in. A sound will play to confirm login success.

6. The onboard LED indicates the number of connected users as described above.

## Customization

You can customize the WiFi SSID and password by modifying the following lines in `src/main.cpp`:

```cpp
const char* ssid = "XI RPL 3";      // Change this to your desired SSID
const char* password = "";           // Set a password or leave empty for open network
```

After making changes, rebuild and upload the firmware to your ESP8266 board.

## Technical Details

- The ESP8266 runs a DNS server that redirects all DNS queries to the device IP, enabling the captive portal behavior.
- The asynchronous web server serves the login page and static files from the LittleFS filesystem.
- User logins are tracked with a timeout (default 5 minutes) to manage connected user count.
- The onboard LED on pin D2 indicates user presence status:
  - Off: no users connected
  - Steady on: one user connected
  - Blinking faster: multiple users connected, blinking speed increases with user count.

## Troubleshooting

- If the LittleFS filesystem fails to mount, ensure the filesystem image is properly uploaded and the board supports LittleFS.
- If the captive portal does not appear, check that your device is connected to the correct WiFi SSID.
- Use the serial monitor to view debug messages for additional insights.

## Contributing

Contributions are welcome! Please open issues or submit pull requests for improvements or bug fixes.

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
