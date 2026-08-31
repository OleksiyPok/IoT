// src/actions/actions.cpp

#include "actions.h"
#include "../buttons/buttons.h"
#include "../indication/indication.h"
#include <WiFi.h>

// ---------------------------------

// ---------------------------------

void handleActions(uint8_t &buttonsState, uint8_t &ledState) {

  if (buttonsState & BUTTON_LIGHT_MASK) {
    ledState |= LED_LIGHT_MANUAL;
  } else {
    ledState &= ~LED_LIGHT_MANUAL;
  }

  if (buttonsState & BUTTON_SERIAL_MONITOR_MASK) {
    ledState |= LED_SERIAL_MONITOR;
  } else {
    ledState &= ~LED_SERIAL_MONITOR;
  }

  if (buttonsState & BUTTON_WIFI_DISABLE) {
    // Serial.println("~~~~~~~~~~~~~~~~~~~~~~");
    Serial.println("[TEST] WiFi disconnect");
    // Serial.println("~~~~~~~~~~~~~~~~~~~~~~");
    WiFi.disconnect();
  }
};