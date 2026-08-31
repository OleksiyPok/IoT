// src/telemetry/telemetry_serializer.h

#pragma once

#include <stddef.h>

#include "telemetry.h"

bool serializeTelemetry(const Telemetry &data, char *buffer, size_t bufferSize);