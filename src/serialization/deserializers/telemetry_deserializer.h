// src/serialization/deserializers/telemetry_deserializer.h

#pragma once

#include <stddef.h>

#include "../../telemetry/telemetry.h"

// ---------------------------------

bool deserializeTelemetry(const char *buffer, size_t bufferSize,
                          Telemetry &data);