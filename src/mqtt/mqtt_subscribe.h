// src/mqtt/mqtt_subscribe.h

#pragma once

#include <Arduino.h>

#include "../serialization/deserializers/sensors_deserializer.h"

// ---------------------------------
void initMqttSubscribe();
bool subscribeMqttTopics();
void handleMqttMessage(char *topic, uint8_t *payload, unsigned int length);

const MqttSensorData &getMqttSensorData();