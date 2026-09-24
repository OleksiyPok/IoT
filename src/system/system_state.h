// src/system/system_state.h

#pragma once

#include "../telemetry/telemetry.h"

// ---------------------------------

#define SYSTEM_SILENT_MASK (1U << 0)
#define SYSTEM_LDR_ERR_MASK (1U << 1) // LDR error
#define SYSTEM_DHT_ERR_MASK (1U << 2) // DHT22 error

#define SYSTEM_TIME_ERR_MASK (1U << 5) // Time synchronization error
#define SYSTEM_MQTT_ERR_MASK (1U << 6) // MQTT error
#define SYSTEM_WIFI_ERR_MASK (1U << 7) // Wi-Fi error

#define SYSTEM_COMMAND_MASK (1U << 8)

#define SYSTEM_STATE_MANAGED_MASK (SYSTEM_SILENT_MASK | SYSTEM_COMMAND_MASK)

#define SYSTEM_STATE_ERR_INIT_MASK                                             \
  (SYSTEM_LDR_ERR_MASK | SYSTEM_DHT_ERR_MASK | SYSTEM_TIME_ERR_MASK |          \
   SYSTEM_MQTT_ERR_MASK | SYSTEM_WIFI_ERR_MASK)

// ---------------------------------

void updateSystemState(Telemetry &telemetry);