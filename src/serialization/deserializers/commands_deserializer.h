#pragma once

#include "../../commands_in/commands_in.h"
#include <stddef.h>

// ---------------------------------

bool deserializeCommand(const char *topic, const char *buffer,
                        size_t bufferSize, IncomingCommand &command);