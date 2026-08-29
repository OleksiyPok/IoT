// src/ldr_sensor/ldr_sensor.h

#pragma once

#include <Arduino.h>

#define STATUS_LDR_OK 0b00000000
#define STATUS_LDR_DEVICE_ERROR 0b00000001
#define STATUS_LDR_DATA_VALID_ERROR 0b00000010

#define STATUS_LDR_LUX_ALARM_MIN 0b00010000
#define STATUS_LDR_LUX_ALARM_MAX 0b00100000

struct LDRData {
  uint16_t raw; // ADC data (0–4095)
  float lux;    // data in the lux
  uint8_t status;
};

void initLdrSensor();
void handleLdrSensor(LDRData &data, uint8_t &ledState);
