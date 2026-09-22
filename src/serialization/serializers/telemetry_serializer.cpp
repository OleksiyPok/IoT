// src/serialization/serializers/telemetry_serializer.cpp

#include <Arduino.h>
#include <inttypes.h>

#include "telemetry_serializer.h"

// ---------------------------------
bool serializeTelemetrySnprintf(const Telemetry &telemetry, char *buffer,
                                size_t bufferSize) {

  if (buffer == nullptr || bufferSize == 0) {
    return false;
  }

  int length = snprintf(
      buffer, bufferSize,
      "{\"version\":%u,"
      "\"deviceId\":%" PRIu64 ","
      "\"timestamp\":%" PRIu32 ","
      "\"uptime\":%" PRIu32 ","
      "\"sequence\":%u,"
      "\"type\":\"telemetry\","
      "\"dht\":{"
      "\"temperature\":%.1f,"
      "\"humidity\":%.1f,"
      "\"updated\":%" PRIu32 ","
      "\"dht_status\":%u"
      "},"
      "\"ldr\":{"
      "\"raw\":%u,"
      "\"lux\":%.1f,"
      "\"updated\":%" PRIu32 ","
      "\"ldr_status\":%u"
      "},"
      "\"sys_state\":%u}",
      telemetry.version, telemetry.deviceId, telemetry.timestamp,
      telemetry.uptime, telemetry.sequence, telemetry.dht.temperature,
      telemetry.dht.humidity, telemetry.dht.updated, telemetry.dht.status,
      telemetry.ldr.raw, telemetry.ldr.lux, telemetry.ldr.updated,
      telemetry.ldr.status, telemetry.systemState);

  // Serial.printf("[SERIALIZER] Sensors size: %d bytes, buffer: %u bytes\r\n",
  //               length, bufferSize);

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
