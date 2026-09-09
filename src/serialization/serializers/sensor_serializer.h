// src/serialization/serializers/sensor_serializer.h

#pragma once

#include <stddef.h>

bool serializeTemperature(float value, char *buffer, size_t bufferSize);

bool serializeHumidity(float value, char *buffer, size_t bufferSize);

bool serializeLux(float value, char *buffer, size_t bufferSize);