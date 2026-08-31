// src/wifi/wifi.cpp

#include <Arduino.h>
#include <WiFi.h>

#include "wifi.h"

// ---------------------------------

static uint32_t wifiConnectStartedAt = 0;
static uint32_t wifiLastReconnectAt = 0;
static bool wifiConnecting = false;
static wl_status_t wifiLastStatus = WL_IDLE_STATUS;

void printWifiStatus(wl_status_t status);

// ---------------------------------

bool isWifiConnected() { return WiFi.status() == WL_CONNECTED; }

void handleWiFi() {
  const uint32_t now = millis();
  const wl_status_t status = WiFi.status();

  if (status == WL_CONNECTED) {
    if (wifiConnecting) {
      Serial.println("[Wi-Fi] Connected");
      Serial.print("[Wi-Fi] IP: ");
      Serial.println(WiFi.localIP());
      wifiConnecting = false;
    }
    wifiLastStatus = status;
    return;
  }

  if (wifiConnecting) {
    if (now - wifiConnectStartedAt >= WIFI_TIMEOUT_MS) {
      Serial.println("[Wi-Fi] Connection timeout");
      WiFi.disconnect();
      wifiConnecting = false;
      wifiLastReconnectAt = now;
      wifiLastStatus = WiFi.status();
    }
    return;
  }

  if (status != wifiLastStatus) {
    Serial.print("[Wi-Fi] Status: ");
    Serial.print(status);
    printWifiStatus(status);
    wifiLastStatus = status;
  }

  if (now - wifiLastReconnectAt < WIFI_RECONNECT_INTERVAL_MS) {
    return;
  }

  connectWifi();
}

bool connectWifi() {
  if (WiFi.status() == WL_CONNECTED || wifiConnecting) {
    return false;
  }

  const uint32_t now = millis();

  wifiLastReconnectAt = now;
  wifiConnectStartedAt = now;
  wifiConnecting = true;

  Serial.println("[Wi-Fi] Connecting...");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD, 6);

  return true;
}

void printWifiStatus(wl_status_t status) {
  switch (status) {
  case WL_IDLE_STATUS:
    Serial.println(" (IDLE)");
    break;
  case WL_NO_SSID_AVAIL:
    Serial.println(" (NO_SSID)");
    break;
  case WL_SCAN_COMPLETED:
    Serial.println(" (SCAN_COMPLETED)");
    break;
  case WL_CONNECTED:
    Serial.println(" (CONNECTED)");
    break;
  case WL_CONNECT_FAILED:
    Serial.println(" (CONNECT_FAILED)");
    break;
  case WL_CONNECTION_LOST:
    Serial.println(" (CONNECTION_LOST)");
    break;
  case WL_DISCONNECTED:
    Serial.println(" (DISCONNECTED)");
    break;
  default:
    Serial.println(" (UNKNOWN)");
    break;
  }
}