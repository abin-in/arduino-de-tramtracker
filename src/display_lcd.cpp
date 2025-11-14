/* SPDX-License-Identifier: MIT
 *
 * display_lcd.cpp
 *
 * Implementation for a 20x4 I2C character LCD.
 *
 * Layout:
 *  - Up to four rows are used to show departures.
 *  - Each row follows the pattern:
 *      [route][space][destination][space][minutes]
 *
 *  - Column allocation:
 *      * Route field:      4 characters (left aligned)
 *      * Destination field: uses remaining space in the middle
 *      * Minutes field:    3 characters, right aligned (e.g. " 1m", "10m")
 */

#include "display_lcd.h"

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#include "network.h"

static const uint8_t LCD_ADDR = 0x27;  // Common default address for PCF8574 I2C backpacks.
static const uint8_t LCD_COLS = 20;
static const uint8_t LCD_ROWS = 4;

// Column widths for individual fields.
static const uint8_t COL_WIDTH_ROUTE = 4;
static const uint8_t COL_WIDTH_MIN   = 3;

// LCD instance.
static LiquidCrystal_I2C lcd(LCD_ADDR, LCD_COLS, LCD_ROWS);

/**
 * @brief Format minutes as a 3-character string.
 *
 * Examples:
 *  - 1  -> " 1m"
 *  - 9  -> " 9m"
 *  - 10 -> "10m"
 *  - 99 -> "99m"
 */
static String formatMinutes(uint8_t minutes) {
  if (minutes > 99) {
    minutes = 99;
  }
  String result = String(minutes) + "m";
  while (result.length() < COL_WIDTH_MIN) {
    result = " " + result;
  }
  return result;
}

/**
 * @brief Render a single departure row at the given LCD row index.
 */
static void renderDepartureRow(uint8_t rowIndex, const Departure& dep) {
  lcd.setCursor(0, rowIndex);

  // Route field.
  char routeBuf[COL_WIDTH_ROUTE + 1];
  snprintf(routeBuf, sizeof(routeBuf), "%-4.4s", dep.line);
  lcd.print(routeBuf);

  lcd.print(' ');

  // Destination field.
  const uint8_t availableDestCols =
      LCD_COLS - COL_WIDTH_ROUTE - COL_WIDTH_MIN - 2;  // two spaces
  char destBuf[availableDestCols + 1];
  snprintf(destBuf, sizeof(destBuf), "%-*.*s",
           availableDestCols, availableDestCols, dep.destination);
  lcd.print(destBuf);

  lcd.print(' ');

  // Minutes field.
  lcd.print(formatMinutes(dep.etaMinutes));
}

/**
 * @brief Render a placeholder row when no data is available.
 */
static void renderNoDataRow(uint8_t rowIndex) {
  lcd.setCursor(0, rowIndex);
  const char* text = "No data";
  lcd.print(text);

  const uint8_t len = strlen(text);
  for (uint8_t c = len; c < LCD_COLS; ++c) {
    lcd.print(' ');
  }
}

void displayLcdInit() {
  lcd.init();
  lcd.backlight();
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("DE Tram Tracker");
}

void displayLcdRender() {
  size_t count = 0U;
  const Departure* list = networkGetDepartures(count);

  if (count == 0U) {
    lcd.clear();
    for (uint8_t r = 0; r < LCD_ROWS; ++r) {
      renderNoDataRow(r);
    }
    return;
  }

  lcd.clear();

  const uint8_t rowsToRender = (count < LCD_ROWS) ? count : LCD_ROWS;
  for (uint8_t r = 0; r < rowsToRender; ++r) {
    renderDepartureRow(r, list[r]);
  }

  for (uint8_t r = rowsToRender; r < LCD_ROWS; ++r) {
    renderNoDataRow(r);
  }
}
