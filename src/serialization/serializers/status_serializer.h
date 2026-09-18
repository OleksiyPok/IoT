// src/serialization/serializers/status_serializer.h

#pragma once

#include <stddef.h>

#include "../../telemetry/telemetry.h"

// ---------------------------------
bool serializeStatusSnprintf(const Telemetry &data, char *buffer,
                             size_t bufferSize);
