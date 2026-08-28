// src/buttons/buttons.cpp

#include "buttons.h"
#include "bit_utils.h"
#include "config.h"
#include <Arduino.h>

// ---------------------------------

uint32_t lastDebounce0 = 0;
uint32_t lastDebounce1 = 0;

volatile bool button0Pressed = false; // Button 0 press flag
volatile bool button1Pressed = false; // Button 1 press flag

uint8_t buttonsHandled = 0x00; // Buttons press handled flag

void IRAM_ATTR onButton0Press();
void IRAM_ATTR onButton1Press();

// ---------------------------------

void initButtons() {
  pinMode(BUTTON_0_PIN, INPUT_PULLUP);
  pinMode(BUTTON_1_PIN, INPUT_PULLUP);

  attachInterrupt(BUTTON_0_PIN, onButton0Press, FALLING);
  attachInterrupt(BUTTON_1_PIN, onButton1Press, FALLING);
}

void handleButtons(uint8_t &buttonsState) {
  uint32_t now = millis();

  if (button0Pressed) {
    button0Pressed = false;
    if (!isBitSet(buttonsHandled, 0) && (now - lastDebounce0 >= DEBOUNCE)) {
      lastDebounce0 = now;        // Update debounce time
      toggleBit(buttonsState, 0); // Toggle button 0 state
      setBit(buttonsHandled, 0);  // Mark button 0 as handled
    }
  }

  if (button1Pressed) {
    button1Pressed = false;
    if (!isBitSet(buttonsHandled, 1) && (now - lastDebounce0 >= DEBOUNCE)) {
      lastDebounce1 = now;        // Update debounce time
      toggleBit(buttonsState, 1); // Toggle button 1 state
      setBit(buttonsHandled, 1);  // Mark button 1 as handled
    }
  }

  // Reset after button 0 release
  if (isBitSet(buttonsHandled, 0) && digitalRead(BUTTON_0_PIN) == HIGH) {
    clearBit(buttonsHandled, 0);
  }

  // Reset after button 1 release
  if (isBitSet(buttonsHandled, 1) && digitalRead(BUTTON_1_PIN) == HIGH) {
    clearBit(buttonsHandled, 1);
  }
}

void IRAM_ATTR onButton0Press() { button0Pressed = true; }
void IRAM_ATTR onButton1Press() { button1Pressed = true; }