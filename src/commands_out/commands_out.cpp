// src/commands_out/commands_out.cpp

#include "commands_out.h"

// ---------------------------------
static const char *pendingCommand = nullptr;
// ---------------------------------

void setOutgoingCommand(const char *name) { pendingCommand = name; }
const char *getOutgoingCommand() { return pendingCommand; }
void clearOutgoingCommand() { pendingCommand = nullptr; }
