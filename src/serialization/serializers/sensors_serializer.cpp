// src/serialization/serializers/sensors_serializer.cpp

#include <Arduino.h>

#include "sensors_serializer.h"

// ---------------------------------

bool serializeSensors(const Telemetry &data, char *buffer, size_t bufferSize) {

  if (buffer == nullptr || bufferSize == 0) {
    return false;
  }

  int length =
      snprintf(buffer, bufferSize,
               "{\"version\":%u,"
               "\"deviceId\":%" PRIu64 ","
               "\"timestamp\":%" PRIu32 ","
               "\"sequence\":%u,"
               "\"dht_temperature\":%.1f,"
               "\"dht_humidity\":%.1f,"
               "\"dht_updated\":%" PRIu32 ","
               "\"ldr_raw\":%u,"
               "\"ldr_lux\":%.1f,"
               "\"ldr_updated\":%" PRIu32 "}",

               data.version, data.deviceId, data.timestamp, data.sequence,
               data.dht.temperature, data.dht.humidity, data.dht.updated,
               data.ldr.raw, data.ldr.lux, data.ldr.updated);

  if (length < 0 || static_cast<size_t>(length) >= bufferSize) {
    buffer[0] = '\0';
    return false;
  }

  return true;
}
