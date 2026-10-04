// src/serialization/serializers/telemetry_serializer.h

#pragma once

#include <stddef.h>

#include "../../telemetry/telemetry.h"

// ---------------------------------
bool serializeTelemetrySnprintf(const Telemetry &data, uint8_t sequence,
                                char *buffer, size_t bufferSize);
