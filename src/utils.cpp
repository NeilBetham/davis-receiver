#include "utils.h"

#include "logging.h"
#include "timing.h"
#include "registers/sysctl.h"
#include "registers/timers.h"

static volatile bool sleep_interrupt = false;
void sleep(uint32_t milliseconds) {
  uint32_t start_count = sys_now();
  uint32_t end_count = start_count + milliseconds;
  uint32_t current_count = start_count;
  sleep_interrupt = false;

  while(current_count >= start_count && current_count < end_count && sleep_interrupt == false) {
    for(uint32_t index = 0; index < 1000; index++);
    current_count = sys_now();
  }
}

void sleep_int() {
  sleep_interrupt = true;
}



static volatile bool u_sleeping = false;
void Timer0A_ISR() {
  u_sleeping = false;
  TIMER_0_ICR |= BIT_0;
}

void usleep(uint32_t microseconds) {
	if(SYSCTL_RCGCTIMER != 0x000000FF){ SYSCTL_RCGCTIMER = 0x000000FF; }
  u_sleeping = true;

	// Disable the timer so we can load the new time count
	TIMER_0_CTL  &= ~(BIT_0);
	TIMER_0_CFG   = 0x00000000;
	TIMER_0_TAMR |= 0x00000001;
	TIMER_0_TAILR = 120 * microseconds;
	TIMER_0_IMR  |= BIT_0;
  TIMER_0_CTL  |= BIT_0;

  while(u_sleeping);
}
