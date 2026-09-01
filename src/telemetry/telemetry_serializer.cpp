// src/telemetry/telemetry_serializer.cpp

#include <Arduino.h>
#include <inttypes.h>

#include "telemetry_serializer.h"

// ---------------------------------
bool serializeTelemetry(const Telemetry &data, char *buffer,
                        size_t bufferSize) {

  if (buffer == nullptr || bufferSize == 0) {
    return false;
  }

  int length =
      snprintf(buffer, bufferSize,
               "{\"deviceId\":%" PRIu64 ","
               "\"timestamp\":%" PRIu32 ","
               "\"uptime\":%" PRIu32 ","
               "\"sequence\":%u,"
               "\"dht\":{"
               "\"temperature\":%.1f,"
               "\"humidity\":%.1f,"
               "\"status\":%u"
               "},"
               "\"ldr\":{"
               "\"raw\":%u,"
               "\"lux\":%.1f,"
               "\"status\":%u"
               "},"
               "\"status\":%u}",
               data.deviceId, data.timestamp, data.uptime, data.sequence,
               data.dht.temperature, data.dht.humidity, data.dht.status,
               data.ldr.raw, data.ldr.lux, data.ldr.status, data.status);

  if (length < 0 || static_cast<size_t>(length) >= bufferSize) {
    buffer[0] = '\0';
    return false;
  }

  return true;
}