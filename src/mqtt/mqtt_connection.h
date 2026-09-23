// src/mqtt/mqtt_connection.h

#pragma once

#include <Arduino.h>

// ---------------------------------

void initMqtt();
bool isMqttConnected();
bool handleMqttConnection(Telemetry &telemetry);
bool mqttPublish(const char *topic, const char *payload);
