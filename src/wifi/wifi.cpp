// src/wifi/wifi.cpp

#include "wifi.h"

#include <Arduino.h>
#include <HTTPClient.h>
#include <WiFi.h>

// ---------------------------------

// ---------------------------------

bool connectWifi() {
  Serial.print("[Wi-Fi] Connecting .... ");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD, 6);

  unsigned long start = millis();
  while (WiFi.status() != WL_CONNECTED) {
    if (millis() - start > WIFI_TIMEOUT) {
      Serial.println(".... Connection timeout");
      return false;
    }
    delay(500);
    Serial.print(".");
  }

  Serial.println(" OK");
  Serial.print("[Wi-Fi] IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();
  return true;
}