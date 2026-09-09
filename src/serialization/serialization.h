// src/serialization/serialization.h

#pragma once

#include <stddef.h>

#include "../telemetry/telemetry.h"
#include "deserializers/telemetry_deserializer.h"
#include "serializers/sensor_serializer.h"


// ---------------------------------

bool serializeTelemetry(const Telemetry &data, char *buffer, size_t bufferSize);