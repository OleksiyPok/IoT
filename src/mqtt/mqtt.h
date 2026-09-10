// src/mqtt/mqtt.h

#pragma once

#include "../telemetry/telemetry.h"

void initMqtt();
void handleMqtt(const Telemetry &telemetryData);