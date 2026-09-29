// src/mqtt/mqtt_connection.h

#pragma once

#include <Arduino.h>

#include "../telemetry/telemetry.h"

// ---------------------------------

void initMqtt();
bool isMqttConnected();
bool handleMqttConnection(const Telemetry &telemetry);
bool mqttPublish(const char *topic, const char *payload);
