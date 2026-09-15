// src/mqtt/mqtt_publish.h

#pragma once

#include "../commands/commands.h"
#include "../telemetry/telemetry.h"

// ---------------------------------
void publishSensors(const Telemetry &telemetryData);
void publishStatus(const Telemetry &telemetryData);
bool publishCommands(CommandQueue &commandQueue);
