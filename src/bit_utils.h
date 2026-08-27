// src/bit_utils.h

#pragma once

#include <Arduino.h>

inline void setBit(unsigned char &reg, unsigned char bit) {
  reg |= (1U << bit);
}
inline void clearBit(unsigned char &reg, unsigned char bit) {
  reg &= ~(1U << bit);
}
inline void toggleBit(unsigned char &reg, unsigned char bit) {
  reg ^= (1U << bit);
}
inline bool readBit(unsigned char reg, unsigned char bit) {
  return (reg & (1U << bit)) != 0;
}