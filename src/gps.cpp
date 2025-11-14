/* SPDX-License-Identifier: MIT
 *
 * gps.cpp
 *
 * GPS subsystem implementation for ESP32 DevKit-style boards.
 *
 * Hardware assumptions:
 *  - GPS module (e.g. NEO-6M / NEO-7M / NEO-8M).
 *  - Connected via UART1 on the ESP32.
 *
 * Default pin assignment (can be modified if wiring differs):
 *  - GPS TX -> ESP32 GPIO16 (UART1 RX)
 *  - GPS RX -> ESP32 GPIO17 (UART1 TX, not always required)
 *  - GPS VCC -> 3V3
 *  - GPS GND -> GND
 *
 * Notes:
 *  - Some GPS breakout boards can be powered from 5V, but
 *    check the module rating. The ESP32 I/O lines are 3.3V.
 */

#include "gps.h"

#include <TinyGPSPlus.h>

// UART configuration for GPS.
static const int GPS_UART_NUM   = 1;
static const int GPS_RX_PIN     = 16;      // ESP32 pin connected to GPS TX
static const int GPS_TX_PIN     = 17;      // ESP32 pin connected to GPS RX
static const uint32_t GPS_BAUD  = 9600UL;  // Typical default baud rate

HardwareSerial gpsSerial(GPS_UART_NUM);
static TinyGPSPlus gpsParser;

// Internal storage for the last known location.
static GpsLocation lastLocation = {0.0, 0.0, false};

void gpsInit() {
  gpsSerial.begin(GPS_BAUD, SERIAL_8N1, GPS_RX_PIN, GPS_TX_PIN);

  lastLocation.latitude  = 0.0;
  lastLocation.longitude = 0.0;
  lastLocation.valid     = false;
}

void gpsUpdate() {
  // Consume all available bytes from GPS UART.
  while (gpsSerial.available() > 0) {
    const char c = gpsSerial.read();
    gpsParser.encode(c);
  }

  // Update stored fix when a new valid location is available.
  if (gpsParser.location.isUpdated() && gpsParser.location.isValid()) {
    lastLocation.latitude  = gpsParser.location.lat();
    lastLocation.longitude = gpsParser.location.lng();
    lastLocation.valid     = true;
  }
}

GpsLocation gpsGetLocation() {
  return lastLocation;
}
