// src/buttons/buttons.cpp

#include <Arduino.h>
#include <WiFi.h>

#include "../indication/indication.h"
#include "buttons.h"
#include "config.h"

// ---------------------------------

#define BUTTON_0_MASK BUTTON_LIGHT_MASK
#define BUTTON_1_MASK BUTTON_SELENT_MASK
#define BUTTON_2_MASK BUTTON_WIFI_DISABLE

uint32_t lastDebounce0 = 0;
uint32_t lastDebounce1 = 0;
uint32_t lastDebounce2 = 0;

volatile bool button0Pressed = false; // Button 0 press flag
volatile bool button1Pressed = false; // Button 1 press flag
volatile bool button2Pressed = false; // Button 2 press flag

uint8_t buttonsHandled = 0x00; // Buttons press handled flag

void IRAM_ATTR onButton0Press();
void IRAM_ATTR onButton1Press();
void IRAM_ATTR onButton2Press();

// ---------------------------------

void initButtons() {
  pinMode(BUTTON_0_PIN, INPUT_PULLUP);
  pinMode(BUTTON_1_PIN, INPUT_PULLUP);
  pinMode(BUTTON_2_PIN, INPUT_PULLUP);

  attachInterrupt(BUTTON_0_PIN, onButton0Press, FALLING);
  attachInterrupt(BUTTON_1_PIN, onButton1Press, FALLING);
  attachInterrupt(BUTTON_2_PIN, onButton2Press, FALLING);
}

void handleButtons(uint8_t &buttonsState) {
  uint32_t now = millis();

  if (button0Pressed) {
    button0Pressed = false;

    if (!(buttonsHandled & BUTTON_0_MASK) && now - lastDebounce0 >= DEBOUNCE) {
      lastDebounce0 = now; // Update debounce time

      buttonsState ^= BUTTON_0_MASK;   // Toggled
      buttonsHandled |= BUTTON_0_MASK; // Mark button 0 as handled
    }
  }

  if (button1Pressed) {
    button1Pressed = false;

    if (!(buttonsHandled & BUTTON_1_MASK) && now - lastDebounce1 >= DEBOUNCE) {
      lastDebounce1 = now; // Update debounce time

      buttonsState ^= BUTTON_1_MASK;   // Toggled
      buttonsHandled |= BUTTON_1_MASK; // Mark button 1 as handled
    }
  }

  if (button2Pressed) {
    button2Pressed = false;

    if (!(buttonsHandled & BUTTON_2_MASK) && now - lastDebounce2 >= DEBOUNCE) {
      lastDebounce2 = now; // Update debounce time

      buttonsState ^= BUTTON_2_MASK;   // Toggled
      buttonsHandled |= BUTTON_2_MASK; // Mark button 2 as handled
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

  // Reset after button 2 release
  if ((buttonsHandled & BUTTON_2_MASK) && digitalRead(BUTTON_2_PIN) == HIGH) {
    buttonsState &= ~BUTTON_2_MASK; // uncomm: Released, comm: Fix state
    buttonsHandled &= ~BUTTON_2_MASK;
  }
}

void IRAM_ATTR onButton0Press() { button0Pressed = true; }
void IRAM_ATTR onButton1Press() { button1Pressed = true; }
void IRAM_ATTR onButton2Press() { button2Pressed = true; }