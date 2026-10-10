// src/app_config.h

#pragma once

// ---------------------------------

#define ACTIONS_MS 50
#define BUTTONS_READ_INTERVAL_MS ACTIONS_MS
#define EVENTS_MS ACTIONS_MS
#define INDICATION_CHANGE_INTERVAL_MS ACTIONS_MS
#define WIFI_CHECK_INTERVAL_MS ACTIONS_MS
#define MQTT_CHECK_INTERVAL_MS ACTIONS_MS

#define SENSOR_DHT_READ_INTERVAL_MS 5000
#define SENSOR_LDR_READ_INTERVAL_MS 5000

// Use the smaller interval value for TELEMETRY_UPDATE_INTERVAL_MS
#define TELEMETRY_UPDATE_INTERVAL_MS                                           \
  ((SENSOR_DHT_READ_INTERVAL_MS < SENSOR_LDR_READ_INTERVAL_MS                  \
        ? SENSOR_DHT_READ_INTERVAL_MS                                          \
        : SENSOR_LDR_READ_INTERVAL_MS))

#define SENSOR_STALE_AFTER_CYCLES 2
#define SENSOR_STALE_INTERVAL_MS                                               \
  (TELEMETRY_UPDATE_INTERVAL_MS * SENSOR_STALE_AFTER_CYCLES)

#define DATA_SEND_INTERVAL_MS 5000
#define MQTT_PUBLISH_INTERVAL_MS 30000
#define MEMORY_CHECK_INTERVAL_MS 30000
// Use the same monitor interval as data send
#define DATA_MONITOR_INTERVAL_MS MQTT_PUBLISH_INTERVAL_MS
