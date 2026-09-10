// src/serialization/serializers/sensors_serializer.h

#pragma once

#include <stddef.h>

#include "../../telemetry/telemetry.h"

// ---------------------------------

bool serializeSensors(const Telemetry &data, char *buffer, size_t bufferSize);