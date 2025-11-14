/* SPDX-License-Identifier: MIT
 *
 * arduino-de-tramtracker
 *
 * Main entry point for the ESP32-based tram tracker.
 *
 * Overview:
 *  - Hardware platform: ESP32 DevKit V1 (or similar ESP32 board).
 *  - Subsystems:
 *      * GPS module for current location (NEO-6M / NEO-7M / NEO-8M).
 *      * Wi-Fi connectivity for calling a backend tram API.
 *      * Character LCD or color TFT to show upcoming departures.
 *
 * High-level responsibilities of this file:
 *  - Initialize all subsystems (GPS, network, displays).
 *  - Run a periodic update loop:
 *      * Continuously feed GPS parser.
 *      * Maintain Wi-Fi and API polling via the network module.
 *      * Refresh the user interface (LCD/TFT) at a defined interval.
 *
 * Hardware summary (default assumptions):
 *  - ESP32 DevKit V1:
 *      * GPS:
 *          - GPS TX  -> ESP32 GPIO16 (UART1 RX)
 *          - GPS RX  -> ESP32 GPIO17 (UART1 TX, often unused)
 *          - GPS VCC -> 3V3 (check module requirements)
 *          - GPS GND -> GND
 *      * I2C LCD (20x4):
 *          - SDA -> GPIO21 (default ESP32 SDA)
 *          - SCL -> GPIO22 (default ESP32 SCL)
 *          - VCC -> 5V or 3V3 (depends on module specification)
 *          - GND -> GND
 */

#include <Arduino.h>

#include "gps.h"
#include "network.h"
#include "display_lcd.h"
#include "display_tft.h"

// Select which display is active at build time.
// Enable only one type at a time to keep logic clear.
static const bool USE_LCD_DISPLAY = true;
static const bool USE_TFT_DISPLAY = false;

// UI refresh interval (milliseconds).
// This controls how often the screen is redrawn with updated data.
static const unsigned long LCD_REFRESH_INTERVAL_MS = 2000U;

static unsigned long lastLcdRefresh = 0U;

void setup() {
  Serial.begin(115200);
  delay(500);

  // Initialize GPS subsystem:
  //  - Configures UART for GPS.
  //  - Prepares TinyGPSPlus parser.
  gpsInit();

  // Initialize network subsystem:
  //  - Starts Wi-Fi connection attempt.
  //  - Clears internal departure list.
  networkInit();

  // Initialize display subsystem(s).
  if (USE_LCD_DISPLAY) {
    // 20x4 I2C character LCD.
    displayLcdInit();
  }

  if (USE_TFT_DISPLAY) {
    // Placeholder for 5-inch color TFT.
    // Hardware and library selection can be decided later.
    displayTftInit();
  }
}

void loop() {
  const unsigned long now = millis();

  // 1. Continuously process GPS serial data so that the location
  //    object is kept up-to-date.
  gpsUpdate();

  // 2. Maintain Wi-Fi connectivity and periodically poll the backend API.
  //    This call is non-blocking for most of the time.
  networkLoop();

  // 3. Refresh displays at a defined interval.
  if (now - lastLcdRefresh >= LCD_REFRESH_INTERVAL_MS) {
    lastLcdRefresh = now;

    if (USE_LCD_DISPLAY) {
      displayLcdRender();
    }

    if (USE_TFT_DISPLAY) {
      displayTftRender();
    }
  }
}
