// src/mqtt/mqtt_connection.h

#pragma once

#include <Arduino.h>
#include <PubSubClient.h>

PubSubClient &getMqttClient();
bool isMqttReconnectAllowed(uint32_t now);
void printMqttStatus(int32_t mqttStatus);