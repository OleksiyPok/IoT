// src/serialization/serializers/events_serializer.cpp

#include <Arduino.h>
#include <inttypes.h>
#include <time.h>

#include "events_serializer.h"

// ---------------------------------

bool serializeEventSnprintf(const char *event, const char *value, char *buffer,
                            size_t bufferSize) {
  if (event == nullptr || buffer == nullptr || bufferSize == 0) {
    return false;
  }

  const time_t timestamp = time(nullptr);

  const int length =
      value == nullptr
          ? snprintf(buffer, bufferSize,
                     "{\"timestamp\":%" PRIu32 ",\"event\":\"%s\"}",
                     static_cast<uint32_t>(timestamp), event)
          : snprintf(buffer, bufferSize,
                     "{\"timestamp\":%" PRIu32
                     ",\"event\":\"%s\",\"value\":\"%s\"}",
                     static_cast<uint32_t>(timestamp), event, value);

  if (length < 0 || static_cast<size_t>(length) >= bufferSize) {
    buffer[0] = '\0';
    Serial.println("[SERIALIZER] ERROR: Event serialization failed");
    return false;
  }

  return true;
}

bool serializeLedChangedSnprintf(bool isOn, char *buffer, size_t bufferSize) {
  return serializeEventSnprintf("led_changed", isOn ? "on" : "off", buffer,
                                bufferSize);
}