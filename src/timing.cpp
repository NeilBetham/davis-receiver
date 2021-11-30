#include "timing.h"

#include "registers/core.h"

static uint32_t millisecond_counter = 0;

void SysTick_Handler() {
  millisecond_counter++;
}

void systick_init() {
  CORE_STRELOAD = 0x00000FA0;  // Trigger every millisecond at 16MHz / 4 PIOSC clock
  CORE_STCTRL   = BIT_0 | BIT_1;
}


uint32_t sys_now() {
  return millisecond_counter;
}
