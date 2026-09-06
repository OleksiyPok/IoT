// src/wifi/wifi.cpp

#include <Arduino.h>
#include <WiFi.h>

#include "wifi.h"

// ---------------------------------

static uint32_t wifiConnectStartedAt = 0;
static uint32_t wifiLastReconnectAt = 0;
static bool wifiConnecting = false;
static wl_status_t wlLastStatus = WL_IDLE_STATUS;

void printWifiStatus(wl_status_t wlStatus);

// ---------------------------------

bool isWifiConnected() { return WiFi.isConnected(); }

void handleWiFi() {
  const uint32_t now = millis();
  const wl_status_t wlStatus = WiFi.status();

  if (wlStatus == WL_CONNECTED) {
    if (wifiConnecting) {
      Serial.println("[Wi-Fi] Connected");
      Serial.print("[Wi-Fi] IP: ");
      Serial.println(WiFi.localIP());
      wifiConnecting = false;
    }
    wlLastStatus = wlStatus;
    return;
  }

  if (wifiConnecting) {
    if (now - wifiConnectStartedAt >= WIFI_CONNECTION_TIMEOUT_MS) {
      Serial.println("[Wi-Fi] Connection timeout");
      WiFi.disconnect();
      wifiConnecting = false;
      wifiLastReconnectAt = now;
      wlLastStatus = WiFi.status();
    }
    return;
  }

  if (wlStatus != wlLastStatus) {
    Serial.print("[Wi-Fi] Status: ");
    Serial.print(wlStatus);
    printWifiStatus(wlStatus);
    wlLastStatus = wlStatus;
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

bool disconnectWiFi() { return WiFi.disconnect(); };

void printWifiStatus(wl_status_t wlStatus) {
  switch (wlStatus) {
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