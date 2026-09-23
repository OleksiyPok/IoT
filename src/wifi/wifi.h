// src/wifi/wifi.h

#pragma once

// ---------------------------------

#define WIFI_RECONNECT_INTERVAL_MS 2500
#define WIFI_CONNECTION_TIMEOUT_MS 5000
#define WIFI_RECONNECT_CYCLE_DELAY_MS 300000
#define WIFI_MAX_CONNECTION_ATTEMPTS 3

bool connectWifi();
bool initWiFi();
bool disconnectWiFi();
bool isWifiConnected();
void handleWiFi();