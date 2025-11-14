# WIRING.md

## Hardware Wiring and Power Guide

Comprehensive standalone wiring documentation for **arduino-de-tramtracker** using **ESP32 DevKit V1**, **NEO-6M GPS**, and **20x4 I²C LCD**. Designed so that anyone can assemble the hardware without referencing external material.

---

## Overview

This wiring guide describes all hardware connections, pin mappings, and power considerations required to build the device. Every connection lists the module pin name, the ESP32 pin name, and its function.

---

## ESP32 DevKit V1 — Core Controller

The ESP32 DevKit V1 functions as the central controller. It manages Wi-Fi connectivity, power distribution, and communication with the GPS module and display hardware.

---

## GPS Module — NEO-6M / NEO-7M / NEO-8M Wiring

The GPS receiver communicates with the ESP32 over the second hardware UART interface (UART2).

**Connections**
- GPS **TX** → ESP32 **GPIO16 (RX2)** — Sends NMEA location data from GPS to ESP32
- GPS **RX** → ESP32 **GPIO17 (TX2)** — Allows optional configuration commands from ESP32 to GPS
- GPS **VCC** → ESP32 **3.3V** — Power supply for GPS logic
- GPS **GND** → ESP32 **GND** — Common ground reference

**Notes**
- Most NEO-6M boards require **3.3V logic**; verify before wiring.
- Antenna must have open sky visibility for stable satellite lock.

---

## LCD Display — 20×4 I²C (PCF8574 Backpack)

This display uses the I²C communication bus and requires only two signal wires.

**Connections**
- LCD **SDA** → ESP32 **GPIO21 (SDA)** — I²C serial data
- LCD **SCL** → ESP32 **GPIO22 (SCL)** — I²C serial clock
- LCD **VCC** → ESP32 **5V** — Power supply to LCD module and backlight
- LCD **GND** → ESP32 **GND** — Shared ground reference

**Notes**
- Default I²C address commonly **0x27** (may vary by backpack version).
- No separate wiring is needed for contrast or backlight control.

---

## Optional 5-Inch Color TFT Display (Future Expansion)

Reserved for later graphical display support. No wiring required in the current phase.

**Reference SPI signals available**
- **MOSI**, **MISO**, **SCK**, **CS**, **DC**, **RESET**

---

## Power Supply Requirements

- System can be powered via **USB 5V** or a stable **regulated 5V DC supply**.
- Recommended power capacity at least **1A** to support ESP32, LCD, and GPS.
- All modules must share a **common ground** for stable communication.
- Use USB for development, external 5V for permanent installation.

**Important**
- Never power GPS from 5V unless documented for your board revision.
- Verify polarity and wiring before powering to avoid component damage.

---

## System Wiring Summary (Readable Text Form)

The ESP32 communicates with the GPS module through UART2 using pins GPIO16 and GPIO17, powered from 3.3V. The LCD display is connected over the I²C bus using GPIO21 for SDA and GPIO22 for SCL, powered from 5V. All components share a common ground line. Wi-Fi connectivity is handled internally by the ESP32.

---

## Final Notes

- Confirm wiring connections before applying power.
- Secure modules and cables within an enclosure for long-term operation.
- GPS performance depends heavily on antenna placement and sky visibility.
- TFT display wiring will be added in future revisions.

---

End of WIRING.md
