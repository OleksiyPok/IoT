// src/serialization/deserializers/commands_deserializer.h

#pragma once

#include <stddef.h>

// ---------------------------------
bool deserializeCommand(const char *buffer, size_t bufferSize, char *command,
                        size_t commandSize);