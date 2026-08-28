// src/bit_utils.h

#pragma once

#include <Arduino.h>

// With single bit number
inline void setBit(uint8_t &reg, uint8_t bit) { reg |= (1U << bit); }
inline void clearBit(uint8_t &reg, uint8_t bit) { reg &= ~(1U << bit); }
inline void toggleBit(uint8_t &reg, uint8_t bit) { reg ^= (1U << bit); }
inline bool isBitSet(uint8_t reg, uint8_t bit) {
  return (reg & (1U << bit)) != 0;
}

// With bit mask
inline void setBitsMask(uint8_t &reg, uint8_t mask) { reg |= mask; }
inline void clearBitsMask(uint8_t &reg, uint8_t mask) { reg &= ~mask; }
inline void toggleBitsMask(uint8_t &reg, uint8_t mask) { reg ^= mask; }
inline bool areBitsSetMask(uint8_t reg, uint8_t mask) {
  return (reg & mask) == mask;
}