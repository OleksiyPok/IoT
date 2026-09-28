// src/mqtt/mqtt_publish.h

#pragma once

#include "../commands/commands.h"
#include "../telemetry/telemetry.h"

// ---------------------------------

bool publishStatus(const Telemetry &telemetry);
bool publishTelemetry(const Telemetry &telemetry);
bool publishCommands();
