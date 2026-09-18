// src/serialization/serializers/status_serializer.cpp

#include "status_serializer.h"

// ---------------------------------
bool serializeStatusSnprintf(const Telemetry &telemetry, char *buffer,
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
               "\"type\":\"statuses\","
               "\"dht_status\":%u,"
               "\"ldr_status\":%u,"
               "\"sys_status\":%u}",
               telemetry.version, telemetry.deviceId, telemetry.timestamp,
               telemetry.uptime, telemetry.sequence, telemetry.dht.status,
               telemetry.ldr.status, telemetry.status);

  // Serial.printf("[SERIALIZER] Status size: %d bytes, buffer: %u bytes\r\n",
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
