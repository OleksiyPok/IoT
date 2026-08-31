#pragma once

#include "telemetry.h"
#include <stddef.h>

bool serializeTelemetryArduinoJson(const Telemetry &telemetryData, char *buffer,
                                   size_t bufferSize);