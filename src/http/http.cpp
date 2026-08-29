// src/http/http.cpp

#include "http.h"
#include "../wifi/wifi.h"
#include <Arduino.h>

// ---------------------------------
void handleSendData(Telemetry &telemetryData);
// ---------------------------------

void handleSendData(Telemetry &telemetryData) {
  if (!isWifiConnected()) {
    connectWifi();
  };
};