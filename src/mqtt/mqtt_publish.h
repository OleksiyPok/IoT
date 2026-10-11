// src/mqtt/mqtt_publish.h

#pragma once

#include "../telemetry/telemetry.h"

// ---------------------------------

bool publishStatus(const Telemetry &telemetry);
bool publishTelemetry(const Telemetry &telemetry);
bool publishCommands();

bool publishLedChanged(bool isOn);
bool publishEvent(const char *event, const char *value = nullptr);
