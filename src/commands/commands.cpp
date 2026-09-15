// src/commands/commands.cpp

#include "commands.h"

// ---------------------------------
static const char *pendingCommand = nullptr;
// ---------------------------------

void setCommand(const char *name) { pendingCommand = name; }
const char *getCommand() { return pendingCommand; }
void clearCommand() { pendingCommand = nullptr; }
