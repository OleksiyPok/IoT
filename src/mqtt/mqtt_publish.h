// src/mqtt/mqtt_publish.h

#pragma once

#include "../commands/commands.h"
#include "../telemetry/telemetry.h"

// ---------------------------------
bool publishSensors(const Telemetry &telemetryData);
bool publishStatus(const Telemetry &telemetryData);

bool publishCommands();
