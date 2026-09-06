// src/monitor/monitor.h

#pragma once

#include "../config.h"
#include "../telemetry/telemetry.h"

// ---------------------------------

// Serial Monitor Output Format
#define PRINT_MODE_DEVICEID 1
#define PRINT_MODE_TIMESTAMP 2
#define PRINT_MODE_UPTIME 3
#define PRINT_MODE_TELEMETRY_DATA 4
#define PRINT_MODE_TELEMETRY_STATUS 5
#define PRINT_MODE_TELEMETRY_DATA_STATUS 6
#define PRINT_MODE_LDR_DATA_STATUS 7
#define PRINT_MODE_DHT_DATA_STATUS 8
#define PRINT_MODE_LDR_DATA_STATUS_DHT_DATA_STATUS 9
#define PRINT_MODE_TELEMETRY_STATUS_DHT_STATUS_LDR_STATUS 10
#define PRINT_MODE_TELEMETRY_ALL_DHT_ALL_LDR_ALL 11
#define PRINT_MODE_BUTTON_STATE_LED_STATE 12

#if defined(DEBUG_MODE)
#define CURRENT_PRINT_MODE PRINT_MODE_TELEMETRY_ALL_DHT_ALL_LDR_ALL
#else
#define CURRENT_PRINT_MODE PRINT_MODE_TELEMETRY_DATA_STATUS
#endif
// ---------------------------------

void initMonitor();

void handleMonitor(const Telemetry &data, const uint8_t &buttonsState,
                   const uint8_t &systemState);

void handleMonitorPayload(const char *payload);