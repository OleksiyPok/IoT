// src/serialization/deserializers/sensors_deserializer.h

#pragma once

#include <stddef.h>

// ---------------------------------
struct MqttSensorData {
  float temperature;
  float ldrLux;
  bool hasTemperature;
  bool hasLdrLux;
};

// ---------------------------------
bool deserializeSensors(const char *buffer, size_t bufferSize,
                        MqttSensorData &data);