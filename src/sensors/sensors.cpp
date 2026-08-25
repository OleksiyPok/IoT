#include "sensors.h"
#include <Arduino.h>

void readSensor(SensorData *data) {
  data->temperature = random(15, 31);
  data->humidity = random(30, 66);
  data->worktime = millis();
}
