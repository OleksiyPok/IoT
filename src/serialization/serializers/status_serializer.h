// src/serialization/serializers/status_serializer.h

#pragma once

#include <stddef.h>

#include "../../telemetry/telemetry.h"

// ---------------------------------

bool serializeStatus(const Telemetry &data, char *buffer, size_t bufferSize);