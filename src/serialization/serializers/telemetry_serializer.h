// src/serialization/serializers/telemetry_serializer.h

#pragma once

#include <stddef.h>

#include "../../telemetry/telemetry.h"

// ---------------------------------

bool serializeTelemetrySnprintf(const Telemetry &data, char *buffer,
                                size_t bufferSize);