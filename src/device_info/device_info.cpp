// src/device_info/device_info.cpp

#include "device_info.h"
#include "esp_mac.h"
#include <Arduino.h>

// --------------------------------------------------
// Get WiFi STA MAC
// --------------------------------------------------

bool getDeviceId(uint64_t &deviceId) {
  uint8_t mac[6];

  if (esp_read_mac(mac, ESP_MAC_WIFI_STA) != ESP_OK) {
    Serial.println("Failed to get device ID");
    deviceId = 000000000000;
    return false;
  }

  deviceId = ((uint64_t)mac[0] << 40) | ((uint64_t)mac[1] << 32) |
             ((uint64_t)mac[2] << 24) | ((uint64_t)mac[3] << 16) |
             ((uint64_t)mac[4] << 8) | ((uint64_t)mac[5]);

  return true;
}

// --------------------------------------------------
// Print device information
// --------------------------------------------------

void printDeviceInfo() {
  uint8_t mac[6];

  if (esp_read_mac(mac, ESP_MAC_WIFI_STA) != ESP_OK) {
    Serial.println("WiFi STA MAC: ERROR");
    return;
  }

  Serial.printf("WiFi STA MAC: %02X:%02X:%02X:%02X:%02X:%02X\r\n", mac[0],
                mac[1], mac[2], mac[3], mac[4], mac[5]);
}