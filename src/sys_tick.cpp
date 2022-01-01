#include "sys_tick.h"

#include "registers/core.h"

#define MAX_SYSTICK_DELEGATES 20

static uint32_t systick_millisecond_counter;
static SysTickDelegate* systick_delegates[MAX_SYSTICK_DELEGATES] = {0};


void SysTick_Handler() {
  systick_millisecond_counter++;
  for(uint32_t index = 0; index < MAX_SYSTICK_DELEGATES; index++) {
    if(systick_delegates[index] != 0) {
      systick_delegates[index]->systick_tick();
    }
  }
}

void systick_init() {
  CORE_STRELOAD = 0x00000FA0;  // Trigger every millisecond at 16MHz / 4 PIOSC clock
  CORE_STCTRL   = BIT_0 | BIT_1;
}

void systick_run() {
  for(uint32_t index = 0; index < MAX_SYSTICK_DELEGATES; index++) {
    if(systick_delegates[index] != 0) {
      systick_delegates[index]->systick_run();
    }
  }
}

bool systick_register_delegate(SysTickDelegate* delegate) {
  for(uint32_t index = 0; index < MAX_SYSTICK_DELEGATES; index++) {
    if(systick_delegates[index] == 0) {
      systick_delegates[index] = delegate;
      return true;
    }
  }

  return false;
}

void systick_delete_delegate(SysTickDelegate* delegate) {
  for(uint32_t index = 0; index < MAX_SYSTICK_DELEGATES; index++) {
    if(systick_delegates[index] == delegate) {
      systick_delegates[index] = 0;
    }
  }
}

uint32_t sys_now() {
  return systick_millisecond_counter;
}
