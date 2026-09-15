// src/commands/commands.h

#pragma once

#include <Arduino.h>

// ---------------------------------
static const char MANUAL_READ_COMMAND[] = "manual_read";
// ---------------------------------

void setCommand(const char *name);
const char *getCommand();
void clearCommand();
