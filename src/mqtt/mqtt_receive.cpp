// src/mqtt/mqtt_receive.cpp

#include <Arduino.h>

#include "../commands_in/commands_in.h"
#include "../project_config.h"
#include "../serialization/serialization.h"
#include "mqtt_receive.h"

// ---------------------------------

void handleMqttMessage(const char *topic, const uint8_t *payload,
                       size_t length) {

  IncomingCommand command;

  if (deserializeCommand(topic, reinterpret_cast<const char *>(payload), length,
                         command)) {
    setIncomingCommand(command);
  }

  //   if (topic == nullptr || payload == nullptr || length == 0) {
  //     return;
  //   }

  // #if defined(DEBUG_MODE)

  //   Serial.println("[MQTT] Received from topic: ");
  //   Serial.print("       \"");
  //   Serial.println(topic);
  //   Serial.println("\"");
  //   Serial.print("[MQTT] Payload: ");

  //   for (size_t i = 0; i < length; ++i) {
  //     Serial.write(payload[i]);
  //   }

  //   Serial.println();

  // #endif
}
