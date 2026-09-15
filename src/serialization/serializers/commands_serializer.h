// src/serialization/serializers/commands_serializer.h

#pragma once

#include <stddef.h>
#include "../../commands/commands.h"

// ---------------------------------
bool serializeCommands(const Command &command, char *buffer, size_t bufferSize);
