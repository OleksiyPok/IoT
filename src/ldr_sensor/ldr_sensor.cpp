// src/ldr_sensor/ldr_sensor.cpp

#include "ldr_sensor.h"
#include "bit_utils.h"
#include "config.h"
#include <Arduino.h>

// ---------------------------------

#define LDR_MIN_VALID_ADC 0
#define c 4095

#define LDR_ADC_VALID_MIN 100
#define LDR_ADC_VALID_MAX 4000

#define LDR_ADC_ALARM_MIN 200
#define LDR_ADC_ALARM_MAX 3900

#define STATUS_LDR_OK 0b00000000
#define STATUS_LDR_DEVICE_ERROR 0b00000001
#define STATUS_LDR_DATA_VALID_ERROR 0b00000010

#define STATUS_LDR_ADC_ALARM_MIN 0b00010000
#define STATUS_LDR_ADC_ALARM_MAX 0b00100000

// float adcToLux(int adcValue);

// ---------------------------------

void initLdrSensor() { pinMode(LDR_ADC_PIN, INPUT); };

void handleLdrSensor(LDRData &data) {

  uint16_t raw = analogRead(LDR_ADC_PIN);

  uint8_t status = STATUS_LDR_OK;

  if (raw < LDR_ADC_VALID_MIN || raw > LDR_ADC_VALID_MAX) {
    setBitsMask(status, STATUS_LDR_DATA_VALID_ERROR);
  }

  data.raw = raw;
  data.status = status;
}

// float adcToLux(int adcValue) {
//   // Код ADC -> напруга на AO. У модулі RDIV стоїть зверху, LDR знизу,
//   // тому чим яскравіше світло — тим МЕНША напруга і менший код ADC.
//   float voltage = adcValue / 4096.0f * VCC;
//   // Дільник напруги в зворотний бік: R_LDR = RDIV * V / (VCC - V)
//   float resistance = RDIV * voltage / (VCC - voltage);
//   float lux = pow(RL10 * 1e3 * pow(10, GAMMA) / resistance, (1.0f / GAMMA));
//   return lux;
// }