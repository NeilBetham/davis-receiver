/**
 * @brief Timer for doing things later
 */

#include <stdint.h>

#include "sys_tick.h"

#pragma once

template<typename Target, void(Target::*Function)()>
class Timer : public SysTickDelegate {
public:
  Timer(Target& target, uint32_t delay_ms, bool repeat = false) : _target(target), _delay(delay_ms), _repeat(repeat) {
    systick_register_delegate(this);
  };
  ~Timer() {
    systick_delete_delegate(this);
  }

  void start() {
    _tick_count = 0;
    _running = true;
  };
  void stop() {
    _running = false;
  };
  void reset() {
    _tick_count = 0;
  };

  void systick_tick() {
    if(_tick_count >= _delay) {
      _tick_count = 0;
      if(_running == true) {
        _should_exec = true;
      }

      if(_repeat == false) {
        _running = false;
      }
    } else {
      _tick_count++;
    }
  };

  void systick_run() {
    if(_should_exec) {
      (_target.*Function)();
      _should_exec = false;
    }
  }

private:
  Target& _target;
  volatile uint32_t _delay = 0;
  volatile bool _repeat = false;

  volatile bool _running = false;
  volatile uint32_t _tick_count = 0;
  volatile bool _should_exec = false;
};
