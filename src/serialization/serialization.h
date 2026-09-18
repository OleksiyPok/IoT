// src/serialization/serialization.h

#pragma once

#include "../telemetry/telemetry.h"
#include <stddef.h>

// ---------------------------------
bool serializeTelemetry(const Telemetry &telemetry, char *buffer,
                        size_t bufferSize);
bool serializeSensors(const Telemetry &telemetry, char *buffer,
                      size_t bufferSize);
bool serializeStatus(const Telemetry &telemetry, char *buffer,
                     size_t bufferSize);
bool serializeCommands(const char *commands, char *buffer, size_t bufferSize);