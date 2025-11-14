/* SPDX-License-Identifier: MIT
 *
 * network.h
 *
 * Network and API subsystem interface.
 *
 * Responsibilities:
 *  - Connect to Wi-Fi.
 *  - Periodically resolve the current GPS location.
 *  - Call a backend tram API and parse its response.
 *  - Provide a read-only list of upcoming departures for display modules.
 */

#pragma once

#include <Arduino.h>

/**
 * @brief Represents a single tram departure.
 */
struct Departure {
  char line[8];           ///< Route or line identifier (e.g. "M4", "M10").
  char destination[40];   ///< Destination name.
  uint8_t etaMinutes;     ///< Minutes until departure (0..255).
};

/**
 * @brief Initialize Wi-Fi and internal API polling state.
 *
 * This function:
 *  - Starts connecting to the configured Wi-Fi network.
 *  - Resets the internal departure buffer.
 */
void networkInit();

/**
 * @brief Network state machine to be called from loop().
 *
 * This function:
 *  - Monitors Wi-Fi connection status.
 *  - If connected and a valid GPS fix is available, periodically:
 *      * Builds a request path using the current coordinates.
 *      * Sends an HTTPS GET request to the backend.
 *      * Parses and stores the list of departures.
 */
void networkLoop();

/**
 * @brief Get the current list of departures.
 *
 * @param count Output parameter that will receive the number of valid entries.
 * @return Pointer to a read-only array of Departure elements.
 */
const Departure* networkGetDepartures(size_t& count);

/**
 * @brief Indicates whether the device is currently connected to Wi-Fi.
 *
 * @return true if the ESP32 has an active Wi-Fi connection, false otherwise.
 */
bool networkIsOnline();
