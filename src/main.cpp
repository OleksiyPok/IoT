#include <Arduino.h>

#define SENSOR_READ_PERIOD_MS 2000   // No RAM is allocated
#define DATA_MONITOR_PERIOD_MS 20000 // No RAM is allocated
#define MEMORY_CHECK_PERIOD_MS 60000 // No RAM is allocated

#define BUILTIN_LED_PIN 2  // No RAM is allocated
#define LED_PIN 4          // No RAM is allocated
#define LED_ON_TIME_MS 100 // No RAM is allocated

unsigned long lastSensorReadMs = 0;  // .bss (Global, zero-initialized)
unsigned long lastDataMonitorMs = 0; // .bss (Global, zero-initialized)
unsigned long lastMemoryCheckMs = 0; // .bss (Global, zero-initialized)

struct SensorData { // .bss
  float temperature;
  float humidity;
  unsigned long worktime;
};

SensorData currentSensorData; // .bss

void readSensor(SensorData *data);      // No memory is allocated
void printData(const SensorData *data); // No memory is allocated
void blinkLed(int pin);                 // No memory is allocated
void checkMemory();                     // No memory is allocated

// --------------

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println("=== Device started ==="); // .rodata
  Serial.println();

  pinMode(LED_PIN, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  delay(500);
}

void loop() {
  unsigned long now = millis(); // Stack (Local variable)

  // Sensor reading
  if (now - lastSensorReadMs >= SENSOR_READ_PERIOD_MS) {
    lastSensorReadMs = now; // .bss (Global variable)
    blinkLed(LED_PIN);

    readSensor(&currentSensorData); //  Pass address by pointer. No data copy
  }

  // Data monitor
  if (now - lastDataMonitorMs >= DATA_MONITOR_PERIOD_MS) {
    lastDataMonitorMs = now; // .bss (Global variable)
    blinkLed(LED_BUILTIN);

    printData(&currentSensorData); //  Pass address by pointer. No data copy
  }

  // Memory check
  if (now - lastMemoryCheckMs >= MEMORY_CHECK_PERIOD_MS) {
    lastMemoryCheckMs = now; // .bss (Global variable)

    checkMemory();
  }

  delay(10); // To simplify the simulation process
}

// --------------

void readSensor(SensorData *data) {
  data->temperature = random(15, 31); //  Pass address by pointer. No data copy
  data->humidity = random(30, 66);    //  Pass address by pointer. No data copy
  data->worktime = millis();          //  Pass address by pointer. No data copy
}

void printData(const SensorData *data) {
  static int callCount = 0; // .bss (Global variable)
  callCount++;

  Serial.print(".... Monitoring #"); // .rodata
  Serial.print(callCount);
  Serial.println(" ...."); // .rodata

  Serial.print("Working time: "); // .rodata
  Serial.print(data->worktime);   //  Pass address by pointer. No data copy
  Serial.println(" ms");          // .rodata

  Serial.print("Temperature: ");   // .rodata
  Serial.print(data->temperature); //  Pass address by pointer. No data copy
  Serial.println(" C");            // .rodata

  Serial.print("Humidity: ");   // .rodata
  Serial.print(data->humidity); //  Pass address by pointer. No data copy
  Serial.println(" %");         // .rodata

  Serial.println();

  Serial.print(ESP.getFreeHeap());
  Serial.println(" bytes");                  // .rodata
  Serial.println("......................."); // .rodata

  Serial.println();
}

void blinkLed(int pin) {
  digitalWrite(pin, HIGH);
  delay(LED_ON_TIME_MS);
  digitalWrite(pin, LOW);
}

void checkMemory() {
  Serial.println("------ Free heap ------"); // .rodata
  Serial.print(ESP.getFreeHeap());
  Serial.println(" bytes");                  // .rodata
  Serial.println("-----------------------"); // .rodata
  Serial.println();
}