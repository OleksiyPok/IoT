// src/serialization/serializers/commands_serializer.cpp

#include <Arduino.h>

#include "sensors_serializer.h"

// ---------------------------------

bool serializeCommands(const Telemetry &data, char *buffer, size_t bufferSize) {

  if (buffer == nullptr || bufferSize == 0) {
    return false;
  }

  int length = snprintf(buffer, bufferSize,
                        "{\"version\":%u,"
                        "\"deviceId\":%" PRIu64 ","
                        "\"timestamp\":%" PRIu32 ","
                        "\"uptime\":%" PRIu32 ","
                        "\"sequence\":%u,",

                        data.version, data.deviceId, data.timestamp,
                        data.uptime, data.sequence);

  if (length < 0 || static_cast<size_t>(length) >= bufferSize) {
    buffer[0] = '\0';
    return false;
  }

  return true;
}
