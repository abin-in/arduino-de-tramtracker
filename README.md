# arduino-de-tramtracker

![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)

---
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
## Code Structure Overview
The project is organized using a modular architecture to support maintainability and clear separation of responsibilities.

---
### File and Module Layout
```
arduino-de-tramtracker/
├── arduino-de-tramtracker.ino        # Main control loop, initializes and coordinates modules
├── src/
│   ├── display_lcd.cpp               # LCD display output (20x4 I2C)
│   ├── display_tft.cpp               # TFT display placeholder module
│   ├── network.cpp                   # Wi-Fi + HTTPS + API polling logic
│   └── gps.cpp                       # GPS module parsing and fix management
└── include/
    ├── display_lcd.h                 # LCD interface definitions
    ├── display_tft.h                 # TFT interface definitions
    ├── network.h                     # Network subsystem interface
    └── gps.h                         # GPS subsystem interface
```

---
### Responsibilities
Each module in the project has a clear responsibility to maintain clean structure and easy expansion.
- arduino-de-tramtracker.ino — Main entry point that initializes all components and runs the main loop.
- gps — Handles GPS hardware communication and location parsing.
- network — Manages Wi-Fi connectivity, calls backend API, parses departure responses.
- display_lcd — Renders departure information on a 20x4 I²C LCD.
- display_tft — Placeholder for a future graphical 5-inch display implementation.

---
This project is licensed under the MIT License. See the LICENSE for details.
