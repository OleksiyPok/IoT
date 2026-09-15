// src/serialization/serializers/commands_serializer.h

#pragma once

#include "../../commands/commands.h"
#include <stddef.h>

// ---------------------------------
bool serializeCommands(const char *command, char *buffer, size_t bufferSize);