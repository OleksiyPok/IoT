// src/ldr_sensor/ldr_sensor.h

#pragma once

#include <Arduino.h>

struct LDRData {
  int raw;   // ADC data (0–4095)
  float lux; // data in the lux
  uint8_t status;
};

void initLdrSensor();
void handleLdrSensor(LDRData &data, uint8_t &ledState);
