// src/monitor/monitor.h

#pragma once

#include "../config.h"
#include "../telemetry/telemetry.h"

// ---------------------------------

void initMonitor();
void handleMonitor(const Telemetry &data, const uint16_t &buttonsState,
                   const uint16_t &ledState);
void printMonitorPayload(const char *payload);