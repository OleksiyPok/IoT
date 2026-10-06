#include <ArduinoJson.h>

#include "../../commands_in/commands_in.h"
#include "../../mqtt/mqtt_config.h"
#include "../serialization_config.h"
#include "commands_deserializer.h"

// ---------------------------------

bool deserializeCommand(const char *topic, const char *buffer,
                        size_t bufferSize, IncomingCommand &command) {

  if (topic == nullptr || buffer == nullptr || bufferSize == 0) {
    return false;
  }

  command.target = CommandTarget::None;
  command.action = CommandAction::None;
  command.value = CommandValue::None;

  if (TOPIC_COMMANDS_IN) {
    command.target = CommandTarget::Led;
  } else {
    return false;
  }

  StaticJsonDocument<128> doc;

  DeserializationError error = deserializeJson(doc, buffer, bufferSize);

  if (error) {
    return false;
  }

  const char *action = doc["action"];
  const char *value = doc["value"];

  if (action == nullptr || value == nullptr) {
    return false;
  }

  if (strcmp(action, "set") == 0) {
    command.action = CommandAction::Set;
  } else {
    return false;
  }

  if (strcmp(value, "on") == 0) {
    command.value = CommandValue::On;
  } else if (strcmp(value, "off") == 0) {
    command.value = CommandValue::Off;
  } else {
    return false;
  }

  return true;
}