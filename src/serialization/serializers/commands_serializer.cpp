// src/serialization/serializers/commands_serializer.cpp

#include <Arduino.h>
#include <inttypes.h>

#include "commands_serializer.h"

// ---------------------------------

bool serializeCommands(const Command &command, char *buffer,
                       size_t bufferSize) {

  if (buffer == nullptr || bufferSize == 0) {
    return false;
  }

  const int length = snprintf(buffer, bufferSize,
                              "{\"sequence\":%" PRIu32 ",\"command\":\"%s\"}",
                              command.sequence, command.name);

  if (length < 0 || static_cast<size_t>(length) >= bufferSize) {
    buffer[0] = '\0';
    return false;
  }

  return true;
}