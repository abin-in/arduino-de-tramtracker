/* SPDX-License-Identifier: MIT
 *
 * gps.h
 *
 * GPS subsystem interface.
 *
 * Responsibilities:
 *  - Provide a simple API for:
 *      * Initializing the GPS hardware.
 *      * Feeding the GPS parser with incoming serial data.
 *      * Returning the latest known location and validity state.
 */

#pragma once

#include <Arduino.h>

/**
 * @brief Container for the last known GPS location.
 */
struct GpsLocation {
  double latitude;   ///< Latitude in degrees.
  double longitude;  ///< Longitude in degrees.
  bool valid;        ///< True if the position is based on a valid fix.
};

/**
 * @brief Initialize the GPS hardware and parser.
 *
 * This function is responsible for:
 *  - Configuring the UART interface used by the GPS module.
 *  - Preparing the NMEA parser.
 *
 * Typical wiring for ESP32 DevKit V1 (UART1):
 *  - GPS TX  -> GPIO16 (RX for UART1)
 *  - GPS RX  -> GPIO17 (TX for UART1, often optional)
 *  - GPS VCC -> 3V3 (check module specification)
 *  - GPS GND -> GND
 */
void gpsInit();

/**
 * @brief Process incoming GPS data.
 *
 * This function must be called regularly from loop().
 * It:
 *  - Reads available bytes from the GPS UART.
 *  - Feeds them into the TinyGPSPlus parser.
 *  - Updates the internally stored location when a new fix is available.
 */
void gpsUpdate();

/**
 * @brief Get the most recent GPS location.
 *
 * @return GpsLocation containing latitude, longitude, and validity flag.
 */
GpsLocation gpsGetLocation();
