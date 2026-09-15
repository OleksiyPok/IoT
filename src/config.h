// src/config.h

#pragma once

// ---------------------------------

// Uncomment to see the debug message in the Serial Monitor
// #define DEBUG_MODE

// Telemetry serialization
// Uncomment to use ArduinoJson telemetry serializer
// #define TELEMETRY_SERIALIZER_ARDUINO_JSON

#define BUTTONS_READ_INTERVAL_MS 100
#define ACTIONS_MS 100
#define INDICATION_CHANGE_INTERVAL_MS 100
#define WIFI_CHECK_INTERVAL_MS 200
#define MQTT_PUBLISH_INTERVAL_MS 10000
#define MEMORY_CHECK_INTERVAL_MS 30000

#define DATA_MONITOR_INTERVAL_MS 2000

// ---------------------------------

#define DHT_TEMPERATURE_ALARM_MIN_CONFIG 20
#define DHT_TEMPERATURE_ALARM_MAX_CONFIG 26

#define LDR_LUX_THRESHOLD_LIGHT_LOW_CONFIG 600

// ---------------------------------

// ESP32 pins configuration

#define LDR_ADC_PIN 33
#define DHT_PIN 13

#define BUTTON_0_PIN 18
#define BUTTON_1_PIN 19
#define BUTTON_2_PIN 4
#define BUTTON_3_PIN 5

#define LED_BUILTIN_PIN 2
#define LED_COMMAND_PIN 21
#define LED_LIGHT_AUTO_PIN 32
#define LED_LIGHT_MIN_PIN 26
#define LED_LIGHT_MAX_PIN 25
#define LED_TEMPERATURE_MIN_PIN 12
#define LED_TEMPERATURE_MAX_PIN 14
#define LED_HUMIDITY_MIN_PIN 27
#define LED_SILENT_PIN 22
