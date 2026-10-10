// src/serialization/serializers/events_serializer.cpp

#include <Arduino.h>
#include <inttypes.h>
#include <time.h>

#include "events_serializer.h"

// ---------------------------------
bool serializeLedChangedSnprintf(bool isOn, char *buffer, size_t bufferSize) {
  if (buffer == nullptr || bufferSize == 0) {
    return false;
  }

  const time_t timestamp = time(nullptr);

  const int length =
      snprintf(buffer, bufferSize,
               "{\"timestamp\":%" PRIu32 ","
               "\"event\":\"led_changed\","
               "\"value\":\"%s\"}",
               static_cast<uint32_t>(timestamp), isOn ? "on" : "off");

  if (length < 0) {
    buffer[0] = '\0';
    Serial.println("[SERIALIZER] ERROR: snprintf formatting failed");
    return false;
  }

  if (static_cast<size_t>(length) >= bufferSize) {
    buffer[0] = '\0';
    Serial.printf("[SERIALIZER] ERROR: message too large: %d bytes, buffer: "
                  "%u bytes\r\n",
                  length, static_cast<unsigned int>(bufferSize));
    return false;
  }

  return true;
}
