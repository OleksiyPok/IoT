// src/mqtt/mqtt_publish.h

#pragma once

#include "../telemetry/telemetry.h"

void publishTelemetry(const Telemetry &telemetryData);
void publishSensorTemperature(const Telemetry &telemetryData);
void publishSensorHumidity(const Telemetry &telemetryData);