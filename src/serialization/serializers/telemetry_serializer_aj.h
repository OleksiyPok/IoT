// src/serialization/serializers/telemetry_serializer_aj.h

#pragma once

#include <stddef.h>

#include "../../telemetry/telemetry.h"

// ---------------------------------
bool serializeTelemetryArduinoJson(const Telemetry &telemetry, uint8_t sequence,
                                   char *buffer, size_t bufferSize);
