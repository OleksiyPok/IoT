// src/mqtt/mqtt_connection.h

#pragma once

#include <Arduino.h>

// ---------------------------------
void initMqtt();
bool isMqttConnected();
bool handleMqttConnection();
bool mqttPublish(const char *topic, const char *payload);
bool mqttSubscribe(const char *topic);