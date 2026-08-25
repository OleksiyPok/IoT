
#include "monitor.h"
#include <Arduino.h>

void monitorData(const SensorData *data) {
  static int callCount = 0;
  callCount++;

  Serial.print(".... Monitoring #");
  Serial.print(callCount);
  Serial.println(" ....");

  Serial.print("Working time: ");
  Serial.print(data->worktime);
  Serial.println(" ms");

  Serial.print("Temperature: ");
  Serial.print(data->temperature);
  Serial.println(" C");

  Serial.print("Humidity: ");
  Serial.print(data->humidity);
  Serial.println(" %");

  Serial.println();

  Serial.print(ESP.getFreeHeap());
  Serial.println(" bytes");
  Serial.println(".......................");

  Serial.println();
}

void checkMemory() {
  Serial.println("------ Free heap ------");
  Serial.print(ESP.getFreeHeap());
  Serial.println(" bytes");
  Serial.println("-----------------------");
  Serial.println();
}
