// src/wifi/wifi.cpp

#include "WiFi.h"
#include "../telemetry/telemetry.h"
#include <Arduino.h>
#include <HTTPClient.h>

// ---------------------------------

static unsigned long wifiConnectStartedAt = 0;
static unsigned long wifiLastReconnectAt = 0;
static bool wifiConnecting = false;
static wl_status_t wifiLastStatus = WL_IDLE_STATUS;

// ---------------------------------

bool isWifiConnected() { return WiFi.status() == WL_CONNECTED; }

bool connectWifi() {
  const wl_status_t status = WiFi.status();
  if (status == WL_CONNECTED) {
    if (wifiLastStatus != WL_CONNECTED) {
      Serial.println("[Wi-Fi] Connected");
      Serial.print("[Wi-Fi] IP: ");
      Serial.println(WiFi.localIP());
      wifiLastStatus = WL_CONNECTED;
      wifiConnecting = false;
    }
    return true;
  }

  if (status != wifiLastStatus) {
    Serial.print("[Wi-Fi] Status: ");
    Serial.println(status);
    wifiLastStatus = status;
  }

  const unsigned long now = millis();

  if (wifiConnecting) {
    if (now - wifiConnectStartedAt >= WIFI_TIMEOUT_MS) {
      Serial.print("[Wi-Fi] Connection timeout. Status: ");
      Serial.println(WiFi.status());
      wifiConnecting = false;
    }
    return false;
  }

  if (now - wifiLastReconnectAt < WIFI_RECONNECT_INTERVAL_MS) {
    return false;
  }

  wifiLastReconnectAt = now;
  wifiConnectStartedAt = now;
  wifiConnecting = true;

  Serial.println("[Wi-Fi] Connecting...");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD, 6);

  return false;
}
