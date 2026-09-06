// src/telemetry/telemetry_deserializer.h

#pragma once

#include <stddef.h>

#include "telemetry.h"

// ---------------------------------

bool deserializeTelemetry(const char *buffer, size_t bufferSize,
                          Telemetry &data);