// src/mqtt/mqtt.h

#pragma once

#include "../telemetry/telemetry.h"

void initMqtt();
bool isMqttConnected();
void handleMqtt(const Telemetry &telemetryData);