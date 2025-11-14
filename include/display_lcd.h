/* SPDX-License-Identifier: MIT
 *
 * display_lcd.h
 *
 * Interface for a 20x4 I2C character LCD.
 *
 * Responsibilities:
 *  - Initialize the LCD module.
 *  - Render the current departure list in a concise tabular layout.
 *
 * Hardware assumptions:
 *  - LCD module with I2C backpack (e.g. PCF8574).
 *  - Connection to ESP32 default I2C pins:
 *      * SDA -> GPIO21
 *      * SCL -> GPIO22
 *      * VCC -> 5V or 3V3 (check module rating)
 *      * GND -> GND
 */

#pragma once

#include <Arduino.h>

/**
 * @brief Initialize the I2C character LCD.
 *
 * Configures the I2C bus and the LCD controller, sets backlight,
 * and clears the display.
 */
void displayLcdInit();

/**
 * @brief Render the current list of departures on the LCD.
 *
 * This function:
 *  - Fetches the latest departures from the network module.
 *  - Displays up to four entries simultaneously.
 *  - Shows "No data" if no departures are available.
 */
void displayLcdRender();
