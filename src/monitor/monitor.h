// src/monitor/monitor.h

#pragma once

#include "../config.h"
#include "../telemetry/telemetry.h"

// ---------------------------------

void initMonitor();
void handleMonitor(const Telemetry &data, const uint8_t &buttonsState,
                   const uint8_t &systemState);
void printMonitorPayload(const char *payload);