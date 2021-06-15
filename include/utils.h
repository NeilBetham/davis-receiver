#pragma once

#include "core.h"

// Stupid struct zeroing
static inline void zero(uint32_t count, uint8_t* data) {
  for(uint32_t index = 0; index < count; index++) {
    *data++ = 0;
  }
}

static inline void copy(uint32_t count, const uint8_t* src, uint8_t* dest) {
  for(uint32_t index = 0; index < count; index++) {
    dest[index] = src[index];
  }
}

// Causes the uC to reset
static inline void abort() {
  CORE_APINT &= ~(0xFFFF0000);
  CORE_APINT |=   0x5FA00004;
}

static inline void sleep(uint32_t cycle_count) {
  for(uint32_t index = 0; index < cycle_count; index++) {}
}
