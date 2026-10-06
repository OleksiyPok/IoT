// src/mqtt/mqtt_receive.cpp

#include <Arduino.h>

#include "mqtt_receive.h"

// ---------------------------------

void handleMqttMessage(const char *topic, const uint8_t *payload,
                       size_t length) {

  if (topic == nullptr || payload == nullptr || length == 0) {
    return;
  }

#if defined(DEBUG_MODE)

  Serial.print("[MQTT] Received from topic: ");
  Serial.println(topic);

  Serial.print("[MQTT] Payload: ");

  for (size_t i = 0; i < length; ++i) {
    Serial.write(payload[i]);
  }

  Serial.println();

#endif
}