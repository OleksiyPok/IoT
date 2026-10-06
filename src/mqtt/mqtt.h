// src/mqtt/mqtt.h

#pragma once

#include "../telemetry/telemetry.h"

// ---------------------------------
void initMqtt();
void handleMqttSensors(const Telemetry &telemetry);
void handleMqttCommands();
bool isMqttConnected();