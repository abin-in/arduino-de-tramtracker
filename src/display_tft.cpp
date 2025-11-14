/* SPDX-License-Identifier: MIT
 *
 * display_tft.cpp
 *
 * Placeholder implementation for a 5-inch color display.
 *
 * Notes:
 *  - This file reserves the interface for a future graphical implementation.
 *  - When a specific TFT module is chosen, the corresponding library
 *    (e.g. TFT_eSPI or a vendor-specific driver) can be added here.
 */

#include "display_tft.h"

// Example (not active yet):
// #include <TFT_eSPI.h>
// static TFT_eSPI tft = TFT_eSPI();

void displayTftInit() {
  // Initialize TFT hardware here when a specific display is selected.
  // Example:
  // tft.init();
  // tft.setRotation(1);
  // tft.fillScreen(TFT_BLACK);
}

void displayTftRender() {
  // Implement graphical rendering of departures here once
  // display hardware and library are defined.
}
