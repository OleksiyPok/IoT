// src/monitor/monitor.h

#pragma once

#include "../config.h"
#include "../telemetry/telemetry.h"

// ---------------------------------
void initMonitor();
void handleMonitor(const Telemetry &data, const uint16_t &systemState);

#if defined(DEBUG_MODE)
void printMonitorPayload(const char *payload);
#endif
