// commands_serializer.h

#pragma once

#include <stddef.h>

#include "../../telemetry/telemetry.h"

// ---------------------------------

bool serializeCommands(const Telemetry &data, char *buffer, size_t bufferSize);