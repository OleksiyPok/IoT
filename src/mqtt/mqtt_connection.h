// src/mqtt/mqtt_connection.h

#pragma once

#include <Arduino.h>

bool handleMqttConnection();
bool mqttPublish(const char *topic, const char *payload);