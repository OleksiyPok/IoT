// src/memory/memory.cpp

#include "memory.h"
#include <Arduino.h>
// ---------------------------------

// ---------------------------------

void checkMemory() {
  Serial.println();
  Serial.println("------ Free heap ------");
  Serial.print(ESP.getFreeHeap());
  Serial.println(" bytes");
  Serial.println("-----------------------");
  Serial.println();
}
