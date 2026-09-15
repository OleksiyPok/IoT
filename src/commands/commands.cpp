// src/commands/commands.cpp

#include <string.h>

#include "commands.h"

// ---------------------------------
static const char *pendingCommand = nullptr;
static char receivedCommand[32] = {0};
// ---------------------------------

void setCommand(const char *name) { pendingCommand = name; }
const char *getCommand() { return pendingCommand; }
void clearCommand() { pendingCommand = nullptr; }

void setReceivedCommand(const char *name) {
  if (name == nullptr) {
    receivedCommand[0] = '\0';
    return;
  }

  strncpy(receivedCommand, name, sizeof(receivedCommand) - 1);
  receivedCommand[sizeof(receivedCommand) - 1] = '\0';
}

const char *getReceivedCommand() {
  return receivedCommand[0] != '\0' ? receivedCommand : nullptr;
}

void clearReceivedCommand() { receivedCommand[0] = '\0'; }