// src/monitor/monitor.cpp

#include <Arduino.h>

#include "../config.h"
#include "../telemetry/telemetry.h"
#include "monitor.h"
#include "monitor_payload.h"
#include "monitor_status.h"
#include "monitor_telemetry.h"

// ---------------------------------

void initMonitor() {
  Serial.begin(115200);
  delay(200);

  Serial.println();
  Serial.println("=== Device ESP32 started ===");
  Serial.println();
}

void handleMonitor(const Telemetry &data, const uint8_t &buttonsState,
                   const uint8_t &systemState) {

#if CURRENT_PRINT_MODE == PRINT_MODE_DEVICEID

  printTelemetryUptime(data);
  printDeviceId(data);

#elif CURRENT_PRINT_MODE == PRINT_MODE_TIMESTAMP

  printTelemetryTimestamp(data);

#elif CURRENT_PRINT_MODE == PRINT_MODE_UPTIME

  printTelemetryUptime(data);

#elif CURRENT_PRINT_MODE == PRINT_MODE_TELEMETRY_DATA

  printTelemetryData(data);

#elif CURRENT_PRINT_MODE == PRINT_MODE_TELEMETRY_STATUS

  printTelemetryUptime(data);
  printTelemetryStatus(data.status);

#elif CURRENT_PRINT_MODE == PRINT_MODE_TELEMETRY_DATA_STATUS

  printTelemetryData(data);
  printTelemetryStatus(data.status);

#elif CURRENT_PRINT_MODE == PRINT_MODE_LDR_DATA_STATUS

  printTelemetryUptime(data);
  printLdrData(data);
  printLdrStatus(data.ldr.status);

#elif CURRENT_PRINT_MODE == PRINT_MODE_DHT_DATA_STATUS

  printTelemetryUptime(data);
  printDhtData(data);
  printDhtStatus(data.dht.status);

#elif CURRENT_PRINT_MODE == PRINT_MODE_LDR_DATA_STATUS_DHT_DATA_STATUS

  printTelemetryUptime(data);

  printDhtData(data);
  printDhtStatus(data.dht.status);

  printLdrData(data);
  printLdrStatus(data.ldr.status);

#elif CURRENT_PRINT_MODE == PRINT_MODE_ALL_STATUS

  printTelemetryUptime(data);

  printTelemetryStatus(data.status);
  printLdrStatus(data.ldr.status);
  printDhtStatus(data.dht.status);

#elif CURRENT_PRINT_MODE == PRINT_MODE_BUTTON_STATE_LED_STATE

  printTelemetryUptime(data);
  printButtonsState(buttonsState);
  printLedState(systemState);

#else

  printTelemetryData(data);
  printTelemetryStatus(data.status);

  Serial.println();

  printLdrData(data);
  printLdrStatus(data.ldr.status);

  Serial.println();

  printDhtData(data);
  printDhtStatus(data.dht.status);

  printButtonsState(buttonsState);
  printLedState(systemState);

#endif

  Serial.println("------------");
}

void handleMonitorPayload(const char *payload) { printMonitorPayload(payload); }