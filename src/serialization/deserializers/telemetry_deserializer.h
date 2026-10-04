// src/serialization/deserializers/telemetry_deserializer.h

#pragma once

#include "../../telemetry/telemetry.h"
#include <stddef.h>

// ---------------------------------
bool deserializeTelemetry(const char *buffer, size_t bufferSize,
                          uint8_t sequence, Telemetry &data);
