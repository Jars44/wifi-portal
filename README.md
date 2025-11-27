# WiFi Portal for ESP8266

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)
[![PlatformIO](https://img.shields.io/badge/PlatformIO-ESP8266-blue)](https://platformio.org/)
[![ESP8266](https://img.shields.io/badge/ESP8266-NodeMCU--v2-green)](https://www.espressif.com/en/products/som/esp8266)

A modern, stylish WiFi captive portal for ESP8266 microcontrollers. This project creates an open access point that serves a responsive login page with light/dark mode toggle, user tracking, and LED status indicators.

## Table of Contents

- [WiFi Portal for ESP8266](#wifi-portal-for-esp8266)
  - [Table of Contents](#table-of-contents)
  - [Features](#features)
  - [Prerequisites](#prerequisites)
  - [Installation](#installation)
  - [Usage](#usage)
  - [Customization](#customization)
    - [WiFi Settings](#wifi-settings)
    - [Timeout Settings](#timeout-settings)
    - [LED Pin](#led-pin)
  - [Technical Details](#technical-details)
  - [Troubleshooting](#troubleshooting)
    - [Common Issues](#common-issues)
    - [Debug Tips](#debug-tips)
  - [Project Structure](#project-structure)
  - [Dependencies](#dependencies)
  - [Contributing](#contributing)
  - [License](#license)

## Features

- **Captive Portal**: Automatic redirection to login page for all connected devices
- **Modern UI**: Responsive design with light/dark mode toggle
- **Audio Feedback**: Plays sound on successful login
- **User Management**: Tracks connected users with automatic timeout (5 minutes)
- **Visual Indicators**: Onboard LED shows connection status:
  - Off: No users connected
  - Steady on: 1 user connected
  - Blinking (faster with more users): Multiple users connected
- **Filesystem**: Uses LittleFS for serving static assets
- **Asynchronous Server**: High-performance web server handling

## Prerequisites

- **Hardware**: ESP8266 NodeMCU v2 or compatible board
- **Software**: PlatformIO IDE or CLI
- **USB Cable**: For programming the board

## Installation

1. **Clone the repository**:

   ```bash
   git clone https://github.com/Jars44/wifi-portal.git
   cd wifi-portal
   ```

2. **Open in PlatformIO**:

   - Launch PlatformIO IDE and open the project folder
   - Or use PlatformIO CLI in the project directory

3. **Build and Upload**:

   ```bash
   platformio run --target upload
   ```

4. **Monitor (optional)**:

   ```bash
   platformio device monitor
   ```

## Usage

1. After uploading, the ESP8266 creates a WiFi access point named **"UNKNOWN"** (open network)

2. Connect any device (phone, laptop) to this WiFi network

3. Open a web browser - you'll be automatically redirected to the login page

4. Use the login button to authenticate

5. Enjoy the audio confirmation and observe the LED status

## Customization

### WiFi Settings

Edit `src/main.cpp` to change the access point configuration:

```cpp
const char* ssid = "Your_Custom_SSID";      // Access point name
const char* password = "";                  // Leave empty for open network
```

### Timeout Settings

Modify user session timeout:

```cpp
const unsigned long userTimeout = 5 * 60 * 1000;  // 5 minutes in milliseconds
```

### LED Pin

Change the LED pin if needed:

```cpp
const int ledPin = 2;  // GPIO 2 (D2 on NodeMCU)
```

After changes, rebuild and upload:

```bash
platformio run --target upload
```

## Technical Details

- **DNS Server**: Redirects all queries to device IP for captive portal
- **Async Web Server**: Handles HTTP requests efficiently
- **LittleFS**: Embedded filesystem for static files
- **User Tracking**: Automatic timeout management
- **LED Logic**: Proportional blinking speed based on user count

## Troubleshooting

### Common Issues

- **Filesystem not mounting**: Ensure LittleFS data is uploaded
- **No captive portal**: Verify correct WiFi connection
- **LED not working**: Check pin connections and GPIO number

### Debug Tips

Use serial monitor for detailed logs:

```bash
platformio device monitor
```

## Project Structure

```text
wifi-portal/
├── src/
│   └── main.cpp          # Main firmware
├── data/
│   └── sound.mp3         # Login sound file
├── include/              # Header files
├── lib/                  # Custom libraries
├── test/                 # Unit tests
├── platformio.ini        # Project config
├── LICENSE               # MIT License
└── README.md             # This file
```

## Dependencies

- [ESP Async WebServer](https://github.com/me-no-dev/ESPAsyncWebServer) - Async HTTP server
- [ESPAsyncTCP](https://github.com/me-no-dev/ESPAsyncTCP) - Async TCP library
- LittleFS - Embedded filesystem

## Contributing

Contributions welcome! Please:

1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Test thoroughly
5. Submit a pull request

## License

Licensed under MIT License - see [LICENSE](LICENSE) for details.
