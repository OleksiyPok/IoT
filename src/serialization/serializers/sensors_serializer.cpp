// src/serialization/serializers/sensors_serializer.cpp

#include <Arduino.h>

#include "sensors_serializer.h"

// ---------------------------------
bool serializeSensorsSnprintf(const Telemetry &telemetry, char *buffer,
                              size_t bufferSize) {

  if (buffer == nullptr || bufferSize == 0) {
    return false;
  }

  int length =
      snprintf(buffer, bufferSize,
               "{\"version\":%u,"
               "\"deviceId\":%" PRIu64 ","
               "\"timestamp\":%" PRIu32 ","
               "\"uptime\":%" PRIu32 ","
               "\"sequence\":%u,"
               "\"dht_temperature\":%.1f,"
               "\"dht_humidity\":%.1f,"
               "\"dht_updated\":%" PRIu32 ","
               "\"ldr_raw\":%u,"
               "\"ldr_lux\":%.1f,"
               "\"ldr_updated\":%" PRIu32 "}",

               telemetry.version, telemetry.deviceId, telemetry.timestamp,
               telemetry.uptime, telemetry.sequence, telemetry.dht.temperature,
               telemetry.dht.humidity, telemetry.dht.updated, telemetry.ldr.raw,
               telemetry.ldr.lux, telemetry.ldr.updated);

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
