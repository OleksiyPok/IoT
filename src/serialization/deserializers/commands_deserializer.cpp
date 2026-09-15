// src/serialization/deserializers/commands_deserializer.cpp

#include <ArduinoJson.h>
#include <string.h>

#include "commands_deserializer.h"

// ---------------------------------
#define COMMAND_JSON_DOCUMENT_SIZE 128

// ---------------------------------
bool deserializeCommand(const char *buffer, size_t bufferSize, char *command,
                        size_t commandSize) {
  if (buffer == nullptr || bufferSize == 0 || command == nullptr ||
      commandSize == 0) {
    return false;
  }

  command[0] = '\0';

  StaticJsonDocument<COMMAND_JSON_DOCUMENT_SIZE> doc;
  DeserializationError error = deserializeJson(doc, buffer, bufferSize);

  if (!error) {
    const char *value = doc["command"];
    if (value == nullptr) {
      return false;
    }

    strncpy(command, value, commandSize - 1);
    command[commandSize - 1] = '\0';
    return true;
  }

  const size_t length = strnlen(buffer, bufferSize);
  if (length == 0 || length >= commandSize) {
    return false;
  }

  memcpy(command, buffer, length);
  command[length] = '\0';
  return true;
}