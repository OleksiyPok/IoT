// src/buttons/buttons.cpp

#include "buttons.h"
#include "../indication/indication.h"
#include "config.h"
#include <Arduino.h>

// ---------------------------------

#define BUTTON_0_MASK (1U << 0)
#define BUTTON_1_MASK (1U << 1)

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

void handleButtons(uint8_t &ledState) {
  uint32_t now = millis();

  if (button0Pressed) {
    button0Pressed = false;

    if (!(buttonsHandled & BUTTON_0_MASK) && now - lastDebounce0 >= DEBOUNCE) {
      lastDebounce0 = now;             // Update debounce time
      ledState ^= LED_LIGHT_MANUAL;    // Toggle led
      buttonsHandled |= BUTTON_0_MASK; // Mark button 0 as handled
    }
  }

  if (button1Pressed) {
    button1Pressed = false;

    if (!(buttonsHandled & BUTTON_1_MASK) && now - lastDebounce1 >= DEBOUNCE) {
      lastDebounce1 = now; // Update debounce time
      //
      buttonsHandled |= BUTTON_1_MASK;
    }
  }

  // Reset after button 0 release
  if ((buttonsHandled & BUTTON_0_MASK) && digitalRead(BUTTON_0_PIN) == HIGH) {
    buttonsHandled &= ~BUTTON_0_MASK;
  }

  // Reset after button 1 release
  if ((buttonsHandled & BUTTON_1_MASK) && digitalRead(BUTTON_1_PIN) == HIGH) {
    buttonsHandled &= ~BUTTON_1_MASK;
  }
}

void IRAM_ATTR onButton0Press() { button0Pressed = true; }
void IRAM_ATTR onButton1Press() { button1Pressed = true; }