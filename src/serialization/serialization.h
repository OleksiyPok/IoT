// src/serialization/serialization.h

#pragma once

#include "../commands_in/commands_in.h"
#include "../telemetry/telemetry.h"
#include <stddef.h>

// ---------------------------------
bool serializeTelemetry(const Telemetry &telemetry, uint8_t sequence,
                        char *buffer, size_t bufferSize);
bool serializeStatus(const Telemetry &telemetry, uint8_t sequence, char *buffer,
                     size_t bufferSize);
bool serializeCommands(const char *commands, char *buffer, size_t bufferSize);

bool deserializeCommand(const char *topic, const char *buffer,
                        size_t bufferSize, IncomingCommand &command);

bool serializeLedChanged(const bool isOn, char *buffer, size_t bufferSize);