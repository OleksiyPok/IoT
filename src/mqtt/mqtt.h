// src/mqtt/mqtt.h

#pragma once

#include "../telemetry/telemetry.h"

bool isMqttConnected();
void initMqtt();
void handleMqtt(const Telemetry &telemetryData);