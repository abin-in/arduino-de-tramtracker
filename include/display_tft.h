/* SPDX-License-Identifier: MIT
 *
 * display_tft.h
 *
 * Interface for a future 5-inch color display implementation.
 *
 * Responsibilities:
 *  - Initialize the selected TFT/IPS display controller.
 *  - Provide a rendering function for graphical departure views.
 *
 * The specific display module and driver library (e.g. ILI9488, FT81x)
 * can be selected later without impacting other source files.
 */

#pragma once

#include <Arduino.h>

/**
 * @brief Initialize the 5-inch color display.
 *
 * This function should configure:
 *  - Display resolution and orientation.
 *  - Default background color.
 *  - Font settings.
 */
void displayTftInit();

/**
 * @brief Render the current departure list on the color display.
 *
 * Implementations can choose any layout that fits the panel:
 *  - Larger fonts for visibility from a distance.
 *  - Color coding for routes or urgency.
 */
void displayTftRender();
