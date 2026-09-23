// src/wifi/wifi.cpp

#include <Arduino.h>
#include <WiFi.h>

#include "../indication/indication.h"
#include "../secrets.h"
#include "../system/system_state.h"
#include "../telemetry/telemetry.h"
#include "wifi.h"

// ---------------------------------
static bool wifiInitialized = false;
static uint32_t wifiConnectStartedAt = 0;
static uint32_t wifiLastReconnectAt = 0;
static uint32_t wifiNextConnectionCycleAt = 0;
static uint8_t wifiConnectionAttempts = 0;

static bool wifiConnecting = false;
static wl_status_t wlLastStatus = WL_IDLE_STATUS;

void printWifiStatus(wl_status_t wlStatus);

// ---------------------------------
bool isWifiConnected() { return WiFi.isConnected(); }

bool initWiFi() {
  connectWifi();

  while (wifiConnecting) {
    handleWiFi();
  }

  return isWifiConnected();
}

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

    wifiConnectionAttempts = 0;
    wifiNextConnectionCycleAt = 0;

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

      if (wifiConnectionAttempts >= WIFI_MAX_CONNECTION_ATTEMPTS) {
        wifiNextConnectionCycleAt = now;
        Serial.print("[Wi-Fi] ");
        Serial.print(WIFI_MAX_CONNECTION_ATTEMPTS);
        Serial.println(" attempts failed - waiting 5 minutes");
      }
    }
    return;
  }

  if (wlStatus != wlLastStatus) {
    Serial.print("[Wi-Fi] Status: ");
    Serial.print(wlStatus);
    printWifiStatus(wlStatus);
    wlLastStatus = wlStatus;
  }

  if (wifiNextConnectionCycleAt != 0) {
    if (now - wifiNextConnectionCycleAt < WIFI_RECONNECT_CYCLE_DELAY_MS) {
      return;
    }

    wifiConnectionAttempts = 0;
    wifiNextConnectionCycleAt = 0;
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
  wifiConnectionAttempts++;

  requestIndication(IndicationType::WIFI_CONNECTING);

  Serial.println("[Wi-Fi] Connecting...");

  if (!wifiInitialized) {
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD, 6);
    wifiInitialized = true;
  } else {
    WiFi.reconnect();
  }

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
