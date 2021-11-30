/**
 * @brief Manages the ARM SysTick system
 */

#pragma once

#include <stdint.h>

void systick_init();

// Used by LWIP to get current system time
extern "C" uint32_t sys_now();

