// src/ldr_sensor/ldr_sensor.h

#pragma once

struct LDRData {
  int raw;   // ADC data (0–4095)
  float lux; // data in the lux
};

void initLdrSensor();
