#include "timer.h"

Timer::Timer(uint32_t delay_ms, bool repeat) : _delay(delay_ms), _repeat(repeat) {}

void Timer::start() {
  _tick_count = 0;
  _running = true;
}

void Timer::stop() {
  _running = false;
}

void Timer::set_delegate(TimerDelegate* delegate) {
  _delegate = delegate;
}

void Timer::systick_tick() {
  if(_tick_count >= _delay) {
    _tick_count = 0;
    if(_delegate != 0 && _running == true) {
      _should_exec = true;
    }

    if(_repeat == false) {
      _running = false;
    }
  } else {
    _tick_count++;
  }
}

void Timer::systick_run() {
  if(_delegate && _should_exec) {
    _delegate ->timer_event();
    _should_exec = false;
  }
}
