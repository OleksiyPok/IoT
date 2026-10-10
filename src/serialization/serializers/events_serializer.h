// src/serialization/serializers/events_serializer.h

#pragma once

#include <stddef.h>

// ---------------------------------
bool serializeLedChangedSnprintf(bool isOn, char *buffer, size_t bufferSize);