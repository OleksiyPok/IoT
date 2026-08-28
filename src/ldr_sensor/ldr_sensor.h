// src/ldr_sensor/ldr_sensor.h

#pragma once

#include <Arduino.h>

#define STATUS_DEVICE_ERROR 0b00000000
#define STATUS_VALIDATION_ERROR 0b00000000

struct LDRData {
  int raw;   // ADC data (0–4095)
  float lux; // data in the lux
  uint8_t status;
};

void initLdrSensor();
void handleLdrSensor(LDRData &data);
