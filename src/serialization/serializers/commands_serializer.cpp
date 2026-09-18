// src/serialization/serializers/commands_serializer.cpp

#include <Arduino.h>
#include <inttypes.h>

#include "commands_serializer.h"

// ---------------------------------
bool serializeCommandsSnprintf(const char *commands, char *buffer,
                               size_t bufferSize) {

  if (commands == nullptr || buffer == nullptr || bufferSize == 0) {
    return false;
  }

  const int length =
      snprintf(buffer, bufferSize, "{\"command\":\"%s\"}", commands);

  // Serial.printf("[SERIALIZER] Commands size: %d bytes, buffer: %u bytes\r\n",
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