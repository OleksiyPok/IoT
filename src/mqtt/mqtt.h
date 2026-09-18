// src/mqtt/mqtt.h

#pragma once

#include "../commands/commands.h"
#include "../telemetry/telemetry.h"

// ---------------------------------
void initMqtt();
void handleMqttSensors(const Telemetry &telemetry);
void handleMqttCommands();
bool isMqttConnected();