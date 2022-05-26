#include "registers/core.h"
#include "registers/wdt.h"
#include "registers/sysctl.h"

extern int main(void);

// Symbols from the linker for init purposes
extern unsigned __text_end__;

extern unsigned __data_load__;
extern unsigned __data_start__;
extern unsigned __data_end__;

extern unsigned __bss_start__;
extern unsigned __bss_end__;

// CPP CTor and DTor lists
typedef void (*InitFunc)();
extern InitFunc __preinit_array_start__[];
extern InitFunc __preinit_array_end__[];

extern InitFunc __init_array_start__[];
extern InitFunc __init_array_end__[];

extern InitFunc __fini_array_start__[];
extern InitFunc __fini_array_end__[];


// Setup helpers
void copy_data() {
  // Copy .data into RAM
  unsigned *src, *dest;
  src = &__data_load__;
  dest = &__data_start__;
  while(dest < &__data_end__) {
    *dest++ = *src++;
  }
}

void zero_data() {
  // Zero bss segement
  unsigned *dest, *end;
  dest = &__bss_start__;
  end = &__bss_end__;
  while(dest < end) {
    *dest++ = 0;
  }
}

void call_initializers() {
  auto init_func = __preinit_array_start__;
  while(init_func < __preinit_array_end__) {
    (*init_func)();
    init_func++;
  }

  init_func = __init_array_start__;
  while(init_func < __init_array_end__) {
    (*init_func)();
    init_func++;
  }
}

void call_finalizers() {
  auto final_func = __fini_array_start__;
  while(final_func < __fini_array_end__) {
    (*final_func)();
    final_func++;
  }
}

void start_wdt() {
  // Start WDT
  volatile uint32_t* wdt_ctl = &WDT0_CTL;
  SYSCTL_RCGCWD |= BIT_0;
  for(uint32_t index = 0; index < 1000; index++);
  WDT0_LOAD = 1200000000;  // Ten seconds at max sys clock
  for(uint32_t index = 0; index < 1000; index++);
  WDT0_CTL |= BIT_1;  // Enabled the WDT to reset the system
}

void system_setup() {
  // This should handle any errata or critical system setup before
  // we start executing app code

  // Enable the floating point co-processor
  CORE_CPAC |= 0x00F00000;

  start_wdt();

  // Disable memory write buffer for debug only
  //CORE_ACTLR |= BIT_1;
}

// Do all relevant early system init here
void Reset_Handler(void) {
  // System setup
  copy_data();
  zero_data();
  call_initializers();
  system_setup();

  // Call application main
  main();

  // Call destructors
  call_finalizers();

  // Don't wander if main returns
  while(1);
}

