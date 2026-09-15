// src/system/system_state.h

#pragma once

// ---------------------------------
#define SYSTEM_COMMAND_MASK (1U << 0)
#define SYSTEM_SILENT_MASK (1U << 1)
#define SYSTEM_LDR_ERR_MASK (1U << 2)
#define SYSTEM_DHT_ERR_MASK (1U << 3)
#define SYSTEM_MQTT_ERR_MASK (1U << 6)
#define SYSTEM_WIFI_ERR_MASK (1U << 7)

#define SYSTEM_STATE_MANAGED_MASK                                              \
  (SYSTEM_COMMAND_MASK | SYSTEM_SILENT_MASK | SYSTEM_LDR_ERR_MASK |            \
   SYSTEM_DHT_ERR_MASK | SYSTEM_MQTT_ERR_MASK | SYSTEM_WIFI_ERR_MASK)
