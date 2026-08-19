#include <Arduino.h>

const int LED_PIN = 4;

const int DOT = 200;
const int DASH = 600;

const int SYMBOL_GAP = 200;
const int LETTER_GAP = 600;
const int WORD_GAP = 1400;

void dot() {
  digitalWrite(LED_PIN, HIGH);
  delay(DOT);
  digitalWrite(LED_PIN, LOW);
  Serial.print(".");
}

void dash() {
  digitalWrite(LED_PIN, HIGH);
  delay(DASH);
  digitalWrite(LED_PIN, LOW);
  Serial.print("-");
}

void letter_S() {
  dot();
  delay(SYMBOL_GAP);
  dot();
  delay(SYMBOL_GAP);
  dot();
}

void letter_O() {
  dash();
  delay(SYMBOL_GAP);
  dash();
  delay(SYMBOL_GAP);
  dash();
}

// ---------------

void setup() {
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(115200);
  Serial.println("UART TEST");
}

void loop() {
  letter_S();
  delay(LETTER_GAP);
  letter_O();
  delay(LETTER_GAP);
  letter_S();
  delay(WORD_GAP);
  Serial.println();
}
