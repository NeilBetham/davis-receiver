#pragma once

#include "registers/core.h"

// Stupid struct zeroing
static inline void zero(uint32_t count, void* data) {
  uint8_t* local_data = (uint8_t*)data;
  for(uint32_t index = 0; index < count; index++) {
    *local_data++ = 0;
  }
}

static inline void copy(uint32_t count, const void* src, void* dest) {
  for(uint32_t index = 0; index < count; index++) {
    ((uint8_t*)dest)[index] = ((uint8_t*)src)[index];
  }
}

// Causes the uC to reset
static inline void reset() {
  CORE_APINT &= ~(0xFFFF0000);
  CORE_APINT |=   0x5FA00004;
}

// Systick based sleep
void sleep(uint32_t milliseconds);
void sleep_int();

// GPTM based sleep
void usleep(uint32_t microseconds);
