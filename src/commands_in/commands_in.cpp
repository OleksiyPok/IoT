#include "commands_in.h"

#include "../buttons/buttons.h"

// ---------------------------------

static IncomingCommand pendingCommand = {
    CommandTarget::None, CommandAction::None, CommandValue::None};

// ---------------------------------

void setIncomingCommand(const IncomingCommand &command) {
  pendingCommand = command;
}

const IncomingCommand &getIncomingCommand() { return pendingCommand; }

void clearIncomingCommand() {
  pendingCommand.target = CommandTarget::None;
  pendingCommand.action = CommandAction::None;
  pendingCommand.value = CommandValue::None;
}

// ---------------------------------

void handleIncomingCommands(Telemetry &telemetry) {

  const IncomingCommand &command = getIncomingCommand();

  if (command.target == CommandTarget::None) {
    return;
  }

  if (command.target == CommandTarget::Led &&
      command.action == CommandAction::Set) {

    if (command.value == CommandValue::On) {
      telemetry.buttonsState |= BUTTON_LIGHT_MANUAL_MASK;
    }

    if (command.value == CommandValue::Off) {
      telemetry.buttonsState &= ~BUTTON_LIGHT_MANUAL_MASK;
    }
  }

  clearIncomingCommand();
}