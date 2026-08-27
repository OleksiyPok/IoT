// src/buttons/buttons.cpp

#include "buttons.h"
#include "bit_utils.h"
#include "config.h"
#include <Arduino.h>

#define DEBOUNCE 50

unsigned long lastDebounce0 = 0;
unsigned long lastDebounce1 = 0;

volatile bool button0Pressed = false; // Button 0 press flag
volatile bool button1Pressed = false; // Button 1 press flag

volatile bool button0Handled = false; // Button 0 press handled flag
volatile bool button1Handled = false; // Button 1 press handled flag

void IRAM_ATTR onButton0Press();
void IRAM_ATTR onButton1Press();

void initButtons() {
  pinMode(BUTTON_0_PIN, INPUT_PULLUP);
  pinMode(BUTTON_1_PIN, INPUT_PULLUP);

  attachInterrupt(BUTTON_0_PIN, onButton0Press, FALLING);
  attachInterrupt(BUTTON_1_PIN, onButton1Press, FALLING);
}

void handleButtons(unsigned char *buttonsState) {
  unsigned long now = millis();

  if (button0Pressed) {
    button0Pressed = false;

    if (!button0Handled && (now - lastDebounce0 >= DEBOUNCE)) {
      lastDebounce0 = now;         // Update debounce time
      toggleBit(*buttonsState, 0); // Toggle button 0 state
      button0Handled = true;       // Mark button 0 as handled
    }
  }

  if (button1Pressed) {
    button1Pressed = false;

    if (!button1Handled && (now - lastDebounce1 >= DEBOUNCE)) {
      lastDebounce1 = now;         // Update debounce time
      toggleBit(*buttonsState, 1); // Toggle button 1 state
      button1Handled = true;       // Mark button 1 as handled
    }
  }

  // Reset after button 0 release
  if (button0Handled && digitalRead(BUTTON_0_PIN) == HIGH) {
    button0Handled = false;
  }

  // Reset after button 1 release
  if (button1Handled && digitalRead(BUTTON_1_PIN) == HIGH) {
    button1Handled = false;
  }
}

void IRAM_ATTR onButton0Press() { button0Pressed = true; }
void IRAM_ATTR onButton1Press() { button1Pressed = true; }