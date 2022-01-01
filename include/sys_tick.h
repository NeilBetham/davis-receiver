/**
 * @brief Sys Tick peripheral management
 */

#pragma once

#include <stdint.h>

class SysTickDelegate {
public:
  virtual ~SysTickDelegate() {}

  // This is called from an interrupt context
  virtual void systick_tick() = 0;

  // This is called from a main loop context
  virtual void systick_run() = 0;
};

void systick_init();
void systick_run();

bool systick_register_delegate(SysTickDelegate* delegate);
void systick_delete_delegate(SysTickDelegate* delegate);

// Used by LWIP to get current system time
extern "C" uint32_t sys_now();

