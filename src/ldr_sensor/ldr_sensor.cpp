// src/ldr_sensor/ldr_sensor.cpp

#include "ldr_sensor.h"
#include "../indication/indication.h"
#include "bit_utils.h"
#include "config.h"
#include <Arduino.h>

// ---------------------------------

#define LDR_ADC_VALID_MIN 10
#define LDR_ADC_VALID_MAX 10000
#define LDR_LUX_ALARM_MIN 200
#define LDR_LUX_ALARM_MAX 5000

#define LDR_LUX_THRESHOLD 600

#define STATUS_LDR_OK 0b00000000
#define STATUS_LDR_DEVICE_ERROR 0b00000001
#define STATUS_LDR_DATA_VALID_ERROR 0b00000010

#define STATUS_LDR_LUX_ALARM_MIN 0b00010000
#define STATUS_LDR_LUX_ALARM_MAX 0b00100000

#define GAMMA 0.7f // нахил графіка log(R)/log(lux) — атрибут Wokwi "gamma"
#define RL10 50.0f // опір LDR при 10 lux, кОм — атрибут Wokwi "rl10"
#define RDIV 10000.0f // постійний резистор дільника в модулі LDR, Ом
#define VCC 3.3f      // напруга живлення дільника, В

float adcToLux(const uint16_t &adcValue);
uint8_t validateLdrData(const uint16_t &raw, const float &lux,
                        uint8_t &ledState);
void checkLdrLuxAlarm(const float &lux, uint8_t &status, uint8_t &ledState);
void checkLdrLuxThreshold(const float &lux, uint8_t &ledState);

// ---------------------------------

void initLdrSensor() { pinMode(LDR_ADC_PIN, INPUT); }

void handleLdrSensor(LDRData &data, uint8_t &ledState) {
  uint16_t raw = analogRead(LDR_ADC_PIN);
  float lux = adcToLux(raw);
  uint8_t status = validateLdrData(raw, lux, ledState);

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

uint8_t validateLdrData(const uint16_t &raw, const float &lux,
                        uint8_t &ledState) {
  uint8_t status = STATUS_LDR_OK;

  if (raw < LDR_ADC_VALID_MIN || raw > LDR_ADC_VALID_MAX) {
    clearBit(ledState, LED_LIGHT_MIN_BIT);
    clearBit(ledState, LED_LIGHT_MAX_BIT);
    setBitsMask(status, STATUS_LDR_DATA_VALID_ERROR);
    return status;
  }

  checkLdrLuxAlarm(lux, status, ledState);
  checkLdrLuxThreshold(lux, ledState);

  return status;
}

void checkLdrLuxAlarm(const float &lux, uint8_t &status, uint8_t &ledState) {
  clearBit(ledState, LED_LIGHT_MIN_BIT);
  clearBit(ledState, LED_LIGHT_MAX_BIT);

  if (lux < LDR_LUX_ALARM_MIN) {
    setBit(ledState, LED_LIGHT_MIN_BIT);
    setBitsMask(status, STATUS_LDR_LUX_ALARM_MIN);
  } else if (lux > LDR_LUX_ALARM_MAX) {
    setBit(ledState, LED_LIGHT_MAX_BIT);
    setBitsMask(status, STATUS_LDR_LUX_ALARM_MAX);
  }
}

void checkLdrLuxThreshold(const float &lux, uint8_t &ledState) {
  if (lux > LDR_LUX_THRESHOLD) {
    setBit(ledState, LED_LIGHT_AUTO_BIT);
  } else {
    clearBit(ledState, LED_LIGHT_AUTO_BIT);
  }
}
