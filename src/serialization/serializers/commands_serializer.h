// src/serialization/serializers/commands_serializer.h

#pragma once

#include "../../commands/commands.h"
#include <stddef.h>

// ---------------------------------
bool serializeCommandsSnprintf(const char *commands, char *buffer,
                               size_t bufferSize);