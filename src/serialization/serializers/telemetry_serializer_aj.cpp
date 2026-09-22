// src/serialization/serializers/telemetry_serializer_aj.cpp

#include <ArduinoJson.h>

#include "telemetry_serializer_aj.h"

// ---------------------------------
#define TELEMETRY_JSON_DOCUMENT_SIZE 512

// ---------------------------------
bool serializeTelemetryArduinoJson(const Telemetry &telemetry, char *buffer,
                                   size_t bufferSize) {

  if (buffer == nullptr || bufferSize == 0) {
    return false;
  }

  DynamicJsonDocument doc(TELEMETRY_JSON_DOCUMENT_SIZE);

  doc["version"] = telemetry.version;
  doc["deviceId"] = telemetry.deviceId;
  doc["timestamp"] = telemetry.timestamp;
  doc["uptime"] = telemetry.uptime;
  doc["sequence"] = telemetry.sequence;
  doc["type"] = "statuses";

  JsonObject dht = doc.createNestedObject("dht");
  dht["temperature"] = telemetry.dht.temperature;
  dht["humidity"] = telemetry.dht.humidity;
  dht["updated"] = telemetry.dht.updated;
  dht["dht_status"] = telemetry.dht.status;

  JsonObject ldr = doc.createNestedObject("ldr");
  ldr["raw"] = telemetry.ldr.raw;
  ldr["lux"] = telemetry.ldr.lux;
  ldr["updated"] = telemetry.ldr.updated;
  ldr["ldr_status"] = telemetry.ldr.status;

  doc["sys_state"] = telemetry.systemState;

  if (doc.overflowed()) {
    buffer[0] = '\0';
    return false;
  }

  size_t length = serializeJson(doc, buffer, bufferSize);

  if (length < 0) {
    buffer[0] = '\0';
    Serial.println("[SERIALIZER] ERROR: snprintf formatting failed");
    return false;
  }

  if (static_cast<size_t>(length) >= bufferSize) {
    buffer[0] = '\0';
    Serial.printf(
        "[SERIALIZER] ERROR: message too large: %d bytes, buffer: %u bytes\r\n",
        length, bufferSize);
    return false;
  }

  return true;
}
