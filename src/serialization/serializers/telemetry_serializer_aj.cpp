// src/serialization/serializers/telemetry_serializer_aj.cpp

// src/serialization/serializers/telemetry_serializer_aj.cpp

#include <Arduino.h>
#include <ArduinoJson.h>
#include <inttypes.h>
#include <math.h>

#include "../serialization_config.h"
#include "telemetry_serializer_aj.h"

// ---------------------------------
bool serializeTelemetryArduinoJson(const Telemetry &telemetry, uint8_t sequence,
                                   char *buffer, size_t bufferSize) {
  if (buffer == nullptr || bufferSize == 0) {
    return false;
  }

  buffer[0] = '\0';

  DynamicJsonDocument doc(TELEMETRY_JSON_DOCUMENT_SIZE);

  doc["version"] = telemetry.version;

  char deviceId[21];
  snprintf(deviceId, sizeof(deviceId), "%" PRIu64, telemetry.device_id);
  doc["device_id"] = deviceId;

  doc["timestamp"] = telemetry.timestamp;
  doc["uptime"] = telemetry.uptime;
  doc["sequence"] = sequence;
  doc["type"] = "telemetry";

  JsonObject dht = doc.createNestedObject("dht");
  dht["temperature"] = roundf(telemetry.dht.temperature * 10.0f) / 10.0f;
  dht["humidity"] = roundf(telemetry.dht.humidity * 10.0f) / 10.0f;
  dht["updated"] = telemetry.dht.updated;
  dht["dht_status"] = telemetry.dht.status;

  JsonObject ldr = doc.createNestedObject("ldr");
  ldr["raw"] = telemetry.ldr.raw;
  ldr["lux"] = roundf(telemetry.ldr.lux * 10.0f) / 10.0f;
  ldr["updated"] = telemetry.ldr.updated;
  ldr["ldr_status"] = telemetry.ldr.status;

  doc["sys_state"] = telemetry.systemState;

  if (doc.overflowed()) {
    Serial.println("[SERIALIZER] ERROR: ArduinoJson document overflowed");
    return false;
  }

  const size_t length = measureJson(doc);

  if (length >= bufferSize) {
    Serial.printf(
        "[SERIALIZER] ERROR: message too large: %u bytes, buffer: %u bytes\r\n",
        static_cast<unsigned int>(length),
        static_cast<unsigned int>(bufferSize));
    return false;
  }

  const size_t written = serializeJson(doc, buffer, bufferSize);

  if (written != length) {
    buffer[0] = '\0';
    Serial.println("[SERIALIZER] ERROR: ArduinoJson serialization failed");
    return false;
  }

  return true;
}