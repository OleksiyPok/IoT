// src/serialization/serializers/events_serializer.h

#pragma once

#include <stddef.h>

// ---------------------------------
bool serializeLedChangedSnprintf(bool isOn, char *buffer, size_t bufferSize);
bool serializeEventSnprintf(const char *event, const char *value, char *buffer,
                            size_t bufferSize);