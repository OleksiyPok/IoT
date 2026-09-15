// src/serialization/deserializers/sensors_deserializer.cpp

#include <ArduinoJson.h>

#include "sensors_deserializer.h"

// ---------------------------------
#define SENSORS_JSON_DOCUMENT_SIZE 512

// ---------------------------------
bool deserializeSensors(const char *buffer, size_t bufferSize,
                        MqttSensorData &data) {
  if (buffer == nullptr || bufferSize == 0) {
    return false;
  }

  data.hasTemperature = false;
  data.hasLdrLux = false;

  StaticJsonDocument<SENSORS_JSON_DOCUMENT_SIZE> doc;
  DeserializationError error = deserializeJson(doc, buffer, bufferSize);

  if (error) {
    return false;
  }

  bool found = false;

  if (doc["dht_temperature"].is<float>()) {
    data.temperature = doc["dht_temperature"].as<float>();
    data.hasTemperature = true;
    found = true;
  }

  if (doc["ldr_lux"].is<float>()) {
    data.ldrLux = doc["ldr_lux"].as<float>();
    data.hasLdrLux = true;
    found = true;
  }

  return found;
}