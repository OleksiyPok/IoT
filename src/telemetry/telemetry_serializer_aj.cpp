// src/telemetry/telemetry_serializer_aj.cpp

#include <ArduinoJson.h>

#include "telemetry_serializer_aj.h"

// ---------------------------------

#define TELEMETRY_JSON_DOCUMENT_SIZE 256

// ---------------------------------

bool serializeTelemetryArduinoJson(const Telemetry &telemetryData, char *buffer,
                                   size_t bufferSize) {
  if (buffer == nullptr || bufferSize == 0) {
    return false;
  }

  DynamicJsonDocument doc(TELEMETRY_JSON_DOCUMENT_SIZE);

  doc["deviceId"] = telemetryData.deviceId;
  doc["timestamp"] = telemetryData.timestamp;
  doc["uptime"] = telemetryData.uptime;

  JsonObject dht = doc.createNestedObject("dht");
  dht["temperature"] = telemetryData.dht.temperature;
  dht["humidity"] = telemetryData.dht.humidity;
  dht["status"] = telemetryData.dht.status;

  JsonObject ldr = doc.createNestedObject("ldr");
  ldr["raw"] = telemetryData.ldr.raw;
  ldr["lux"] = telemetryData.ldr.lux;
  ldr["status"] = telemetryData.ldr.status;

  doc["status"] = telemetryData.status;

  if (doc.overflowed()) {
    buffer[0] = '\0';
    return false;
  }

  size_t length = serializeJson(doc, buffer, bufferSize);

  if (length == 0 || length >= bufferSize) {
    buffer[0] = '\0';
    return false;
  }

  return true;
}