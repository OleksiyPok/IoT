// src/commands_out/commands_out.h

#pragma once

#include "commands_out_config.h"

// ---------------------------------

void setOutgoingCommand(const char *name);
const char *getOutgoingCommand();
void clearOutgoingCommand();
