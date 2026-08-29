// src/wifi/wifi.h

#pragma once

#define WIFI_SSID "Wokwi-GUEST" // SSID
#define WIFI_PASSWORD ""        // Password (empty for Wokwi-GUEST)
#define WIFI_RECONNECT_INTERVAL_MS 3000
#define WIFI_TIMEOUT_MS 10000 // timeout connection, ms

#define SERVER_URL "http://httpbun.com/post" // HTTP POST

#define SEND_INTERVAL 30000

bool connectWifi();
bool isWifiConnected();
void updateWifiStatus();
