// src/serialization/serializers/status_serializer.h

#pragma once

#include <stddef.h>

#include "../../telemetry/telemetry.h"

// ---------------------------------
bool serializeStatusSnprintf(const Telemetry &data, uint8_t sequence,
                             char *buffer, size_t bufferSize);
