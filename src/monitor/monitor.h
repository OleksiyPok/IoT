// src/monitor/monitor.h

#pragma once

#include "../config.h"

// ---------------------------------
void initMonitor();
void handleMonitor(const uint16_t &systemState, const uint16_t &buttonsState,
                   const uint16_t &ledState);

#if defined(DEBUG_MODE)
void printMonitorPayload(const char *payload);
#endif
