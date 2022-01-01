/**
 * @brief Timer for doing things later
 */

#include <stdint.h>

#include "sys_tick.h"

#pragma once

class TimerDelegate {
public:
  virtual ~TimerDelegate() {};

  virtual void timer_event() = 0;
};


class Timer : public SysTickDelegate {
public:
  Timer(uint32_t delay_ms, bool repeat = false);

  void start();
  void stop();

  void set_delegate(TimerDelegate* delegate);

  void systick_tick();
  void systick_run();

private:
  volatile uint32_t _delay = 0;
  volatile bool _repeat = false;
  TimerDelegate* _delegate = 0;

  volatile bool _running = false;
  volatile uint32_t _tick_count = 0;
  volatile bool _should_exec = false;
};
