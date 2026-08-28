// src/bit_utils.h

#pragma once

#include <Arduino.h>

inline void setBit(uint8_t &reg, uint8_t bit) { reg |= (1U << bit); }
inline void clearBit(uint8_t &reg, uint8_t bit) { reg &= ~(1U << bit); }
inline void toggleBit(uint8_t &reg, uint8_t bit) { reg ^= (1U << bit); }
inline bool readBit(uint8_t reg, uint8_t bit) {
  return (reg & (1U << bit)) != 0;
}