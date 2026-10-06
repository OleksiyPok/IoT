#pragma once

#include "../telemetry/telemetry.h"

// ---------------------------------

enum class CommandTarget { None, Led };

enum class CommandAction { None, Set };

enum class CommandValue { None, On, Off };

struct IncomingCommand {
  CommandTarget target;
  CommandAction action;
  CommandValue value;
};

// ---------------------------------

void setIncomingCommand(const IncomingCommand &command);
const IncomingCommand &getIncomingCommand();
void clearIncomingCommand();

void handleIncomingCommands(Telemetry &telemetry);