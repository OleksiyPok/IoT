// src/telemetry/telemetry_serializer.h

#pragma once

#include "telemetry.h"
#include <stddef.h>

bool serializeTelemetry(const Telemetry &data, char *buffer, size_t bufferSize);