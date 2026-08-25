// src/config.h

#ifndef MAIN_H
#define MAIN_H

#include <Arduino.h>

#define SENSOR_READ_PERIOD_MS 2000
#define DATA_MONITOR_PERIOD_MS 20000
#define MEMORY_CHECK_PERIOD_MS 60000

#define LDR_PIN 33

#define DHTT_PIN 4
#define DHTT_TYPE DHT22

#define BUTTON 15
#define DEBOUNCE 50

#define LED_BUILTIN_PIN 2
#define LED_PIN 4
#define LED_ON_TIME_MS 100

#endif