// src/monitor/monitor_payload.cpp

#include <Arduino.h>
#include <ArduinoJson.h>

#include "monitor_payload.h"
#include "monitor_status.h"
#include "monitor_telemetry.h"

// ---------------------------------

static void printTelemetryPayload(const JsonDocument &doc);
static void printPayloadStatus(const JsonDocument &doc);

// ---------------------------------

void printMonitorPayload(const char *payload) {

#if defined(DEBUG_MODE)

  Serial.println("================================");
  Serial.println("| !!!!!!!! DEBUG MODE !!!!!!!! |");
  Serial.println("|  Comment out ''DEBUG_MODE''  |");
  Serial.println("|    in the configuration      |");
  Serial.println("| to switch to Production mode |");
  Serial.println("================================");

#endif

  if (payload == nullptr) {
    Serial.println("[MONITOR] Payload is null");
    return;
  }

  StaticJsonDocument<512> doc;

  DeserializationError error = deserializeJson(doc, payload);

  if (error) {
    Serial.print("[MONITOR] JSON parse error: ");
    Serial.println(error.c_str());
    return;
  }

  Serial.println();
  Serial.println("[MONITOR] HTTP payload:");

  printTelemetryPayload(doc);
  printPayloadStatus(doc);

  Serial.println("------------");
}

// ---------------------------------

static void printTelemetryPayload(const JsonDocument &doc) {
  const uint64_t deviceId = doc["deviceId"].as<uint64_t>();

  Serial.print("[TELEMETRY] DeviceId (MAC): ");

  Serial.printf("%02X:%02X:%02X:%02X:%02X:%02X\r\n", (uint8_t)(deviceId >> 40),
                (uint8_t)(deviceId >> 32), (uint8_t)(deviceId >> 24),
                (uint8_t)(deviceId >> 16), (uint8_t)(deviceId >> 8),
                (uint8_t)deviceId);

  const uint32_t timestamp = doc["timestamp"].as<uint32_t>();

  time_t timeValue = timestamp;

  struct tm timeInfo;
  gmtime_r(&timeValue, &timeInfo);

  Serial.printf(
      "[TELEMETRY] Timestamp: %04d-%02d-%02d %02d:%02d:%02d UTC (%lu)\r\n",
      timeInfo.tm_year + 1900, timeInfo.tm_mon + 1, timeInfo.tm_mday,
      timeInfo.tm_hour, timeInfo.tm_min, timeInfo.tm_sec,
      static_cast<unsigned long>(timestamp));

  const uint32_t uptime = doc["uptime"].as<uint32_t>();

  const uint32_t hours = uptime / 3600;
  const uint32_t minutes = (uptime % 3600) / 60;
  const uint32_t seconds = uptime % 60;

  Serial.printf(
      "[TELEMETRY] Uptime: %02lu:%02lu:%02lu (%lu sec)\r\n",
      static_cast<unsigned long>(hours), static_cast<unsigned long>(minutes),
      static_cast<unsigned long>(seconds), static_cast<unsigned long>(uptime));

  Serial.print("[TELEMETRY] Sequence: ");
  Serial.println(doc["sequence"].as<uint8_t>());

  Serial.print("[LDR] ADC: ");
  Serial.print(doc["ldr"]["raw"].as<uint16_t>());
  Serial.print(", Lux: ");
  Serial.println(doc["ldr"]["lux"].as<float>(), 1);

  Serial.print("[LDR] Updated: ");
  Serial.println(doc["ldr"]["updated"].as<uint32_t>());

  Serial.print("[DHT] Temperature: ");
  Serial.print(doc["dht"]["temperature"].as<float>(), 1);
  Serial.print("°C,  Humidity: ");
  Serial.print(doc["dht"]["humidity"].as<float>(), 0);
  Serial.println("%");

  Serial.print("[DHT] Updated: ");
  Serial.println(doc["dht"]["updated"].as<uint32_t>());
}

// ---------------------------------

static void printPayloadStatus(const JsonDocument &doc) {
  const uint8_t status = doc["status"].as<uint8_t>();
  const uint8_t ldrStatus = doc["ldr"]["status"].as<uint8_t>();
  const uint8_t dhtStatus = doc["dht"]["status"].as<uint8_t>();

  Serial.println();

  printTelemetryStatus(status);

  Serial.println();

  printLdrStatus(ldrStatus);

  Serial.println();

  printDhtStatus(dhtStatus);
}