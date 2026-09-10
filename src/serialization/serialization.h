// src/serialization/serialization.h

#pragma once

#include <stddef.h>

#include "../telemetry/telemetry.h"

// ---------------------------------

bool serializeTelemetry(const Telemetry &data, char *buffer, size_t bufferSize);
