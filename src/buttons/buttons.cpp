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

unsigned char buttonsHandled = 0x00;

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

    // if (!readBit(buttonsHandled, 0) && (now - lastDebounce0 >= DEBOUNCE)) {
    if (!button0Handled && (now - lastDebounce0 >= DEBOUNCE)) {
      lastDebounce0 = now;         // Update debounce time
      toggleBit(*buttonsState, 0); // Toggle button 0 state
      button0Handled = true;       // Mark button 0 as handled
      // setBit(buttonsHandled, 0);
    }
  }

  if (button1Pressed) {
    button1Pressed = false;

    // if (!readBit(buttonsHandled, 1) && (now - lastDebounce0 >= DEBOUNCE)) {
    if (!button1Handled && (now - lastDebounce1 >= DEBOUNCE)) {
      lastDebounce1 = now;         // Update debounce time
      toggleBit(*buttonsState, 1); // Toggle button 1 state
      button1Handled = true;       // Mark button 1 as handled
      // setBit(buttonsHandled, 1);
    }
  }

  // Reset after button 0 release
  // if (readBit(buttonsHandled, 0) && digitalRead(BUTTON_0_PIN) == HIGH) {
  if (button0Handled && digitalRead(BUTTON_0_PIN) == HIGH) {
    button0Handled = false;
    // clearBit(buttonsHandled, 0);
  }

  // Reset after button 1 release
  // if (readBit(buttonsHandled, 1) && digitalRead(BUTTON_1_PIN) == HIGH) {
  if (button1Handled && digitalRead(BUTTON_1_PIN) == HIGH) {
    button1Handled = false;
    // clearBit(buttonsHandled, 1);
  }
}

void IRAM_ATTR onButton0Press() { button0Pressed = true; }
void IRAM_ATTR onButton1Press() { button1Pressed = true; }