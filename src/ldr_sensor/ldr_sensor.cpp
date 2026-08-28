// src/ldr_sensor/ldr_sensor.cpp

#include "ldr_sensor.h"
#include "config.h"
#include <Arduino.h>

// ---------------------------------

void ldrAdcReadData(LDRData &data);
void validateLdrSensorData(LDRData &data);

// ---------------------------------

void initLdrSensor() { pinMode(LDR_ADC_PIN, INPUT); };

void handleLdrSensor(LDRData &data) {
  ldrAdcReadData(data);
  validateLdrSensorData(data);
};

void ldrAdcReadData(LDRData &data) { data.raw = analogRead(LDR_ADC_PIN); };

void validateLdrSensorData(LDRData &data) {
  //  if (ldrpayload.raw < 0 || ldrpayload.raw > 4095) {
  //   status |= STATUS_LDR_ERR;
}
