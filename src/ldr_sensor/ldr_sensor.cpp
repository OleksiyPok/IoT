// src/ldr_sensor/ldr_sensor.cpp

#include "ldr_sensor.h"
#include "../indication/indication.h"
#include "config.h"
#include <Arduino.h>

// ---------------------------------

// #define LDR_ADC_VALID_MIN 10
// #define LDR_ADC_VALID_MAX 4000
#define LDR_LUX_VALID_MIN 10
#define LDR_LUX_ALARM_MIN 200
#define LDR_LUX_THRESHOLD 600
#define LDR_LUX_ALARM_MAX 5000
#define LDR_LUX_VALID_MAX 80000

#define GAMMA 0.7f // нахил графіка log(R)/log(lux) — атрибут Wokwi "gamma"
#define RL10 50.0f // опір LDR при 10 lux, кОм — атрибут Wokwi "rl10"
#define RDIV 10000.0f // постійний резистор дільника в модулі LDR, Ом
#define VCC 3.3f      // напруга живлення дільника, В

float adcToLux(const uint16_t &adcValue);

// ---------------------------------

void initLdrSensor(LDRData &data) {
  pinMode(LDR_ADC_PIN, INPUT);
  data.status |= STATUS_LDR_INIT_ERR;
}

void handleLdrSensor(LDRData &data, uint8_t &ledState) {
  uint16_t raw = analogRead(LDR_ADC_PIN);
  float lux = adcToLux(raw);

  uint8_t status = 0;

  // Is NaN
  if (isnan(lux)) {

    ledState &= ~(LED_LIGHT_MIN | LED_LIGHT_MAX);
    status |= STATUS_LDR_DEVICE_ERR;

    data.raw = -1;
    data.lux = -1;
    data.status = status;

    return;
  }

  // ADC light validation (in LUX)
  if (lux < LDR_LUX_VALID_MIN || lux > LDR_LUX_VALID_MAX) {
    status |= STATUS_LDR_DATA_VALID_ERR;
  }

  // Light alarm
  if (lux < LDR_LUX_ALARM_MIN) {
    ledState &= ~LED_LIGHT_MAX;
    ledState |= LED_LIGHT_MIN;
    status |= STATUS_LDR_LUX_ALARM_MIN;
  } else if (lux > LDR_LUX_ALARM_MAX) {
    ledState &= ~LED_LIGHT_MIN;
    ledState |= LED_LIGHT_MAX;
    status |= STATUS_LDR_LUX_ALARM_MAX;
  } else {
    ledState &= ~(LED_LIGHT_MIN | LED_LIGHT_MAX);
  }

  // Light threshold
  if (lux < LDR_LUX_THRESHOLD) {
    ledState |= LED_LIGHT_AUTO;
  } else {
    ledState &= ~LED_LIGHT_AUTO;
  }

  data.raw = raw;
  data.lux = lux;
  data.status = status;
}

float adcToLux(const uint16_t &adcValue) {
  float voltage = adcValue / 4096.0f * VCC;
  // R_LDR = RDIV * V / (VCC - V)
  float resistance = RDIV * voltage / (VCC - voltage);
  float lux = pow(RL10 * 1e3 * pow(10, GAMMA) / resistance, (1.0f / GAMMA));
  return lux;
}
