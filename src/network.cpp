/* SPDX-License-Identifier: MIT
 *
 * network.cpp
 *
 * Network and API subsystem implementation for ESP32.
 *
 * Design:
 *  - Uses Wi-Fi as the transport (ESP32 WiFi library).
 *  - Uses WiFiClientSecure for HTTPS communication with a backend service.
 *  - Uses ArduinoJson for JSON parsing.
 *
 * Backend expectations:
 *  - A collaborator is expected to implement a backend (or select an existing
 *    API) that:
 *      * Accepts GPS coordinates (lat, lon).
 *      * Resolves nearest tram stops.
 *      * Returns a normalized list of departures in JSON form.
 *
 * Example JSON format (can be adjusted as needed):
 * [
 *   {"line":"M4","destination":"Hackescher Markt","eta_min":3},
 *   {"line":"M5","destination":"Hauptbahnhof","eta_min":8}
 * ]
 */

#include "network.h"

#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>

#include "gps.h"

// -----------------------------------------------------------------------------
// Wi-Fi configuration
// -----------------------------------------------------------------------------

/*
 * Wi-Fi connection details.
 *
 * These placeholders should be replaced by real values.
 * In a production system, this information might be externalized,
 * e.g. to a separate header file, NVS, or configuration interface.
 */
static const char* WIFI_SSID     = "YOUR_WIFI_SSID";
static const char* WIFI_PASSWORD = "YOUR_WIFI_PASSWORD";

// -----------------------------------------------------------------------------
// Backend API configuration
// -----------------------------------------------------------------------------

/*
 * Backend host configuration.
 *
 * API_HOST:
 *   DNS name or IP address of the backend server.
 *
 * API_PORT:
 *   TCP port number. For HTTPS, this is usually 443.
 *
 * API_USE_TLS:
 *   Indicates whether TLS should be used. For production, this should be true.
 */
static const char* API_HOST      = "your-proxy.example.com";
static const uint16_t API_PORT   = 443;
static const bool API_USE_TLS    = true;

// Frequency of API polling (in milliseconds).
static const unsigned long API_POLL_INTERVAL_MS = 30UL * 1000UL;

// Maximum number of departures stored in memory.
static const uint8_t MAX_DEPARTURES = 8;

// Secure client for HTTPS.
static WiFiClientSecure secureClient;

// Internal storage for departures.
static Departure departureBuffer[MAX_DEPARTURES];
static size_t departureCount = 0U;

// Timestamp of last API poll.
static unsigned long lastApiPoll = 0U;

// Cached Wi-Fi connection flag.
static bool wifiConnected = false;

// -----------------------------------------------------------------------------
// Internal helpers
// -----------------------------------------------------------------------------

/**
 * @brief Clear the internal departure list.
 */
static void clearDepartures() {
  for (size_t i = 0; i < MAX_DEPARTURES; ++i) {
    departureBuffer[i].line[0]        = '\0';
    departureBuffer[i].destination[0] = '\0';
    departureBuffer[i].etaMinutes     = 0U;
  }
  departureCount = 0U;
}

/**
 * @brief Build the backend request path based on a GPS location.
 *
 * Example produced path:
 *   /v1/departures?lat=52.520007&lon=13.404954&minutes=30
 *
 * The actual backend is free to interpret this path and query parameters as it
 * sees fit. The collaborator can adjust this function to match the real API.
 */
static String buildApiPath(const GpsLocation& loc) {
  String path = "/v1/departures?";
  path += "lat=";
  path += String(loc.latitude, 6);
  path += "&lon=";
  path += String(loc.longitude, 6);
  path += "&minutes=30";
  return path;
}

/**
 * @brief Parse departures from a JSON payload.
 *
 * This function assumes a JSON array response, where each element
 * has at least:
 *  - "line"        (string)
 *  - "destination" (string)
 *  - "eta_min"     (integer minutes)
 *
 * The collaborator can adjust field names as needed.
 */
static void parseDeparturesFromJson(const String& json) {
  clearDepartures();

  const size_t CAPACITY =
      JSON_ARRAY_SIZE(16) + 16 * JSON_OBJECT_SIZE(3) + 512;
  StaticJsonDocument<CAPACITY> doc;

  DeserializationError err = deserializeJson(doc, json);
  if (err) {
    return;
  }

  size_t idx = 0U;
  for (JsonObject obj : doc.as<JsonArray>()) {
    if (idx >= MAX_DEPARTURES) {
      break;
    }

    const char* lineField = obj["line"]        | "";
    const char* destField = obj["destination"] | "";
    const int etaField    = obj["eta_min"]     | -1;

    if (etaField < 0 || etaField > 255) {
      continue;
    }

    strncpy(departureBuffer[idx].line,
            lineField,
            sizeof(departureBuffer[idx].line) - 1);
    departureBuffer[idx].line[sizeof(departureBuffer[idx].line) - 1] = '\0';

    strncpy(departureBuffer[idx].destination,
            destField,
            sizeof(departureBuffer[idx].destination) - 1);
    departureBuffer[idx].destination[sizeof(departureBuffer[idx].destination) - 1] = '\0';

    departureBuffer[idx].etaMinutes = static_cast<uint8_t>(etaField);
    ++idx;
  }

  departureCount = idx;
}

/**
 * @brief Perform a single HTTPS GET request to the backend.
 *
 * @param path Request path, including the query string.
 * @param responseBody Output string containing the response body (without headers).
 * @return true on success, false otherwise.
 *
 * Note:
 *  - TLS certificate validation is currently disabled via setInsecure().
 *    This should be hardened in production deployments.
 */
static bool performHttpsGet(const String& path, String& responseBody) {
  responseBody = "";

  if (!API_USE_TLS) {
    return false;
  }

  // Insecure mode: certificate validation is not performed.
  // For production, consider setCACert() or certificate pinning.
  secureClient.setInsecure();

  if (!secureClient.connect(API_HOST, API_PORT)) {
    return false;
  }

  // Construct HTTP/1.1 GET request.
  secureClient.print("GET ");
  secureClient.print(path);
  secureClient.print(" HTTP/1.1\r\nHost: ");
  secureClient.print(API_HOST);
  secureClient.print("\r\nConnection: close\r\n\r\n");

  const unsigned long timeoutMs = 8000UL;
  const unsigned long start     = millis();

  while (secureClient.connected() && (millis() - start < timeoutMs)) {
    while (secureClient.available()) {
      char c = secureClient.read();
      responseBody += c;
    }
  }

  secureClient.stop();

  // Strip HTTP headers.
  int headerEndIndex = responseBody.indexOf("\r\n\r\n");
  if (headerEndIndex >= 0) {
    responseBody = responseBody.substring(headerEndIndex + 4);
  }

  return !responseBody.isEmpty();
}

// -----------------------------------------------------------------------------
// Public interface
// -----------------------------------------------------------------------------

void networkInit() {
  clearDepartures();
  wifiConnected = false;
  lastApiPoll   = 0U;

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
}

void networkLoop() {
  // Update Wi-Fi connection flag.
  if (WiFi.status() == WL_CONNECTED) {
    wifiConnected = true;
  } else {
    wifiConnected = false;
    // A reconnection strategy can be implemented here if needed.
  }

  const unsigned long now = millis();
  if (!wifiConnected) {
    return;
  }

  // Respect API polling interval.
  if (now - lastApiPoll < API_POLL_INTERVAL_MS) {
    return;
  }
  lastApiPoll = now;

  // GPS must provide a valid fix before calling the backend.
  GpsLocation loc = gpsGetLocation();
  if (!loc.valid) {
    clearDepartures();
    return;
  }

  String path   = buildApiPath(loc);
  String body   = "";
  const bool ok = performHttpsGet(path, body);

  if (!ok) {
    clearDepartures();
    return;
  }

  parseDeparturesFromJson(body);
}

const Departure* networkGetDepartures(size_t& count) {
  count = departureCount;
  return departureBuffer;
}

bool networkIsOnline() {
  return wifiConnected;
}
