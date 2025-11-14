# arduino-de-tramtracker

![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)

## Project Description
An Arduino-based device that automatically detects nearby tram stops anywhere in Germany (DE) using GPS coordinates and retrieves real-time arrival information through public transit APIs. Upcoming tram arrival times are displayed on an LCD display or a 5-inch color display

---

## Features
- Automatically detects nearest tram stations using GPS
- Retrieves real-time arrival information via transit APIs
- Displays upcoming arrivals on an LCD display or a 5-inch color display
- Continuous refresh for real-time updates
- Works independently without a mobile phone or app

---

## Hardware Requirements (Initial Draft)
- Arduino board (UNO / Mega / ESP32 recommended for network usage)
- GPS module (e.g., NEO-6M, NEO-7M or higher)
- LCD display (20x4 I2C) or a 5-inch color display
- Wi-Fi / Ethernet / GSM module depending on API connectivity
- Power supply module or USB
- Optional enclosure

---

## Software Requirements
- Arduino IDE
- Required libraries (to be documented later)
  - GPS parsing library
  - Network/HTTP client library
  - Display driver library
  - JSON parsing library

---

## Planned Features / Roadmap
- Nearest station lookup by GPS coordinates
- Multiple station result listing
- Scrollable long text display logic
- Configurable refresh interval
- API provider configuration file
- Support for additional transport types (future)

---

This project is licensed under the MIT License. See the LICENSE for details.
