// src/serialization/serializers/sensor_serializer.cpp

#include <Arduino.h>

#include "sensor_serializer.h"

// ---------------------------------

static bool serializeFloat(float value, char *buffer, size_t bufferSize) {

  if (buffer == nullptr || bufferSize == 0) {
    return false;
  }

  int length = snprintf(buffer, bufferSize, "%.1f", value);

  if (length < 0 || static_cast<size_t>(length) >= bufferSize) {
    buffer[0] = '\0';
    return false;
  }

  return true;
}

// ---------------------------------

bool serializeTemperature(float value, char *buffer, size_t bufferSize) {
  return serializeFloat(value, buffer, bufferSize);
}

// ---------------------------------

bool serializeHumidity(float value, char *buffer, size_t bufferSize) {
  return serializeFloat(value, buffer, bufferSize);
}

// ---------------------------------

bool serializeLux(float value, char *buffer, size_t bufferSize) {
  return serializeFloat(value, buffer, bufferSize);
}