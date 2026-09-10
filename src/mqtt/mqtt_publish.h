// src/mqtt/mqtt_publish.h

#pragma once

#include "../telemetry/telemetry.h"

// void publishTelemetry(const Telemetry &telemetryData);

void publishSensors(const Telemetry &telemetryData);
void publishStatus(const Telemetry &telemetryData);