// src/monitor/monitor_time.cpp

#include <Arduino.h>

#include "clock/clock.h"
#include "monitor_time.h"


// ---------------------------------

// ---------------------------------
void printUtcTime() {
  struct tm utcTime;

  if (getCurrentUtcTime(utcTime)) {
    Serial.printf("%04d-%02d-%02d %02d:%02d:%02d UTC\n", utcTime.tm_year + 1900,
                  utcTime.tm_mon + 1, utcTime.tm_mday, utcTime.tm_hour,
                  utcTime.tm_min, utcTime.tm_sec);
  }
}

void printLocalTime() {
  struct tm localTime;

  if (getLocalTime(localTime)) {
    Serial.printf("%04d-%02d-%02d %02d:%02d:%02d\n", localTime.tm_year + 1900,
                  localTime.tm_mon + 1, localTime.tm_mday, localTime.tm_hour,
                  localTime.tm_min, localTime.tm_sec);
  }
}
