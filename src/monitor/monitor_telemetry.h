// src/monitor/monitor_telemetry.h

#pragma once

#include "../telemetry/telemetry.h"

// ---------------------------------

void printDeviceId(const Telemetry &data);
void printTelemetryTimestamp(const Telemetry &data);
void printTelemetryUptime(const Telemetry &data);
void printTelemetrySequence(const Telemetry &data);

void printTelemetryData(const Telemetry &data);

void printDhtData(const Telemetry &data);
void printLdrData(const Telemetry &data);