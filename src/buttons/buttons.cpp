// src/buttons/buttons.cpp

#include <Arduino.h>
#include <WiFi.h>

#include "../indication/indication.h"
#include "buttons.h"
#include "config.h"

// ---------------------------------

#define BUTTON_0_MASK BUTTON_COMMAND_MASK
#define BUTTON_1_MASK BUTTON_SILENT_MASK
#define BUTTON_2_MASK BUTTON_2
#define BUTTON_3_MASK BUTTON_WIFI_DISABLE_MASK

#define BUTTON_0_FIXED false
#define BUTTON_1_FIXED true
#define BUTTON_2_FIXED false
#define BUTTON_3_FIXED false

// ---------------------------------

uint32_t lastDebounce0 = 0;
uint32_t lastDebounce1 = 0;
uint32_t lastDebounce2 = 0;
uint32_t lastDebounce3 = 0;

volatile bool button0Pressed = false; // Button 0 press flag
volatile bool button1Pressed = false; // Button 1 press flag
volatile bool button2Pressed = false; // Button 2 press flag
volatile bool button3Pressed = false; // Button 2 press flag

uint8_t buttonsHandled = 0x00; // Buttons press handled flag

void IRAM_ATTR onButton0Press();
void IRAM_ATTR onButton1Press();
void IRAM_ATTR onButton2Press();
void IRAM_ATTR onButton3Press();

// ---------------------------------

void initButtons() {
  pinMode(BUTTON_0_PIN, INPUT_PULLUP);
  pinMode(BUTTON_1_PIN, INPUT_PULLUP);
  pinMode(BUTTON_2_PIN, INPUT_PULLUP);
  pinMode(BUTTON_3_PIN, INPUT_PULLUP);

  attachInterrupt(BUTTON_0_PIN, onButton0Press, FALLING);
  attachInterrupt(BUTTON_1_PIN, onButton1Press, FALLING);
  attachInterrupt(BUTTON_2_PIN, onButton2Press, FALLING);
  attachInterrupt(BUTTON_3_PIN, onButton3Press, FALLING);
}

void handleButtons(uint8_t &buttonsState) {
  uint32_t now = millis();

  if (button0Pressed) {
    button0Pressed = false;

    if (!(buttonsHandled & BUTTON_0_MASK) &&
        now - lastDebounce0 >= BUTTON_DEBOUNCE_TIME_MS) {
      lastDebounce0 = now; // Update debounce time

      buttonsState ^= BUTTON_0_MASK;   // Toggled
      buttonsHandled |= BUTTON_0_MASK; // Mark button 0 as handled
    }
  }

  if (button1Pressed) {
    button1Pressed = false;

    if (!(buttonsHandled & BUTTON_1_MASK) &&
        now - lastDebounce1 >= BUTTON_DEBOUNCE_TIME_MS) {
      lastDebounce1 = now; // Update debounce time

      buttonsState ^= BUTTON_1_MASK;   // Toggled
      buttonsHandled |= BUTTON_1_MASK; // Mark button 1 as handled
    }
  }

  if (button2Pressed) {
    button2Pressed = false;

    if (!(buttonsHandled & BUTTON_2_MASK) &&
        now - lastDebounce2 >= BUTTON_DEBOUNCE_TIME_MS) {
      lastDebounce2 = now; // Update debounce time

      buttonsState ^= BUTTON_2_MASK;   // Toggled
      buttonsHandled |= BUTTON_2_MASK; // Mark button 2 as handled
    }
  }

  if (button3Pressed) {
    button3Pressed = false;

    if (!(buttonsHandled & BUTTON_3_MASK) &&
        now - lastDebounce3 >= BUTTON_DEBOUNCE_TIME_MS) {
      lastDebounce3 = now; // Update debounce time

      buttonsState ^= BUTTON_3_MASK;   // Toggled
      buttonsHandled |= BUTTON_3_MASK; // Mark button 3 as handled
    }
  }

  // Reset after button 0 release
  if ((buttonsHandled & BUTTON_0_MASK) && digitalRead(BUTTON_0_PIN) == HIGH) {
    if (!(BUTTON_0_FIXED)) {
      buttonsState &= ~BUTTON_0_MASK;
    }
    buttonsHandled &= ~BUTTON_0_MASK;
  }

  // Reset after button 1 release
  if ((buttonsHandled & BUTTON_1_MASK) && digitalRead(BUTTON_1_PIN) == HIGH) {
    if (!(BUTTON_1_FIXED)) {
      buttonsState &= ~BUTTON_1_MASK;
    }
    buttonsHandled &= ~BUTTON_1_MASK;
  }

  // Reset after button 2 release
  if ((buttonsHandled & BUTTON_2_MASK) && digitalRead(BUTTON_2_PIN) == HIGH) {
    if (!(BUTTON_2_FIXED)) {
      buttonsState &= ~BUTTON_2_MASK;
    }
    buttonsHandled &= ~BUTTON_2_MASK;
  }

  // Reset after button 3 release
  if ((buttonsHandled & BUTTON_3_MASK) && digitalRead(BUTTON_3_PIN) == HIGH) {
    if (!(BUTTON_3_FIXED)) {
      buttonsState &= ~BUTTON_3_MASK;
    }
    buttonsHandled &= ~BUTTON_3_MASK;
  }
}

void IRAM_ATTR onButton0Press() { button0Pressed = true; }
void IRAM_ATTR onButton1Press() { button1Pressed = true; }
void IRAM_ATTR onButton2Press() { button2Pressed = true; }
void IRAM_ATTR onButton3Press() { button3Pressed = true; }