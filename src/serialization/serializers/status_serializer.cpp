// src/serialization/serializers/status_serializer.cpp

#include "status_serializer.h"

// ---------------------------------

bool serializeStatus(const Telemetry &data, char *buffer, size_t bufferSize) {

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
               "\"dht_status\":%u,"
               "\"ldr_status\":%u,"
               "\"system_status\":%u}",
               data.version, data.deviceId, data.timestamp, data.uptime,
               data.sequence, data.dht.status, data.ldr.status, data.status);

  if (length < 0 || static_cast<size_t>(length) >= bufferSize) {
    buffer[0] = '\0';
    return false;
  }

  return true;
}