// src/telemetry/telemetry_serializer_aj.h

#pragma once

#include <stddef.h>

#include "telemetry.h"

bool serializeTelemetryArduinoJson(const Telemetry &telemetryData, char *buffer,
                                   size_t bufferSize);