// src/mqtt/mqtt_receive.h

#pragma once

#include <Arduino.h>

// ---------------------------------

void handleMqttMessage(const char *topic, const uint8_t *payload,
                       size_t length);