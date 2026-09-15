// src/serialization/serializers/commands_serializer.cpp

#include <Arduino.h>
#include <inttypes.h>

#include "commands_serializer.h"

// ---------------------------------
bool serializeCommands(const char *command, char *buffer, size_t bufferSize) {

  if (command == nullptr || buffer == nullptr || bufferSize == 0) {
    return false;
  }

  const int length =
      snprintf(buffer, bufferSize, "{\"command\":\"%s\"}", command);

  if (length < 0 || static_cast<size_t>(length) >= bufferSize) {
    buffer[0] = '\0';
    return false;
  }

  return true;
}