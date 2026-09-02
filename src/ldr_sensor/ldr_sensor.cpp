// src/ldr_sensor/ldr_sensor.cpp

#include <Arduino.h>

#include "../indication/indication.h"
#include "config.h"
#include "ldr_sensor.h"

// ---------------------------------

#define LDR_LUX_VALID_MIN 1
#define LDR_LUX_ALARM_MIN 10
#define LDR_LUX_THRESHOLD_LIGHT_LOW 600
#define LDR_LUX_ALARM_MAX 10000
#define LDR_LUX_VALID_MAX 70000

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

void handleLdrSensor(LDRData &data) {
  uint16_t raw = analogRead(LDR_ADC_PIN);
  float lux = adcToLux(raw);

  uint8_t status = STATUS_LDR_OK;

  // Is NaN
  if (!isfinite(lux) || isnan(lux)) {

    Serial.print("[DEBUG-ERR] Raw: ");
    Serial.println(raw);
    Serial.print("[DEBUG-ERR] Lux: ");
    Serial.println(lux);

    status |= STATUS_LDR_DEVICE_ERR;
    data.status = status;
    return;
  }

  data.updated = millis();

  // ADC light validation (in LUX)
  if (lux < LDR_LUX_VALID_MIN || lux > LDR_LUX_VALID_MAX) {
    status |= STATUS_LDR_DATA_VALID_ERR;
  }

  // Light alarm
  if (lux < LDR_LUX_ALARM_MIN) {
    status |= STATUS_LDR_LUX_ALARM_MIN;
  } else if (lux > LDR_LUX_ALARM_MAX) {
    status |= STATUS_LDR_LUX_ALARM_MAX;
  }

  // Light threshold
  if (lux < LDR_LUX_THRESHOLD_LIGHT_LOW) {
    status |= STATUS_LDR_LIGHT_LOW;
  } else {
    status &= ~STATUS_LDR_LIGHT_LOW;
  }

  data.raw = raw;
  data.lux = lux;
  data.status = status;
}

float adcToLux(const uint16_t &adcValue) {
  // ADC = 0 -> R_LDR = 0 -> division by zero
  if (adcValue == 0) {
    return NAN;
  }

  float voltage = adcValue / 4096.0f * VCC;

  // Prevent division by zero when voltage == VCC
  if (voltage >= VCC) {
    return NAN;
  }

  // R_LDR = RDIV * V / (VCC - V)
  float resistance = RDIV * voltage / (VCC - voltage);

  if (resistance <= 0.0f) {
    return NAN;
  }

  float lux = pow(RL10 * 1e3 * pow(10, GAMMA) / resistance, (1.0f / GAMMA));

  return lux;
}
