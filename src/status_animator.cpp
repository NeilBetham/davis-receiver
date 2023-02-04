#include "status_animator.h"

#include "status_led.h"

#include <math.h>

constexpr uint32_t HALF_PROGRESS = 0.5 * UINT32_MAX;
constexpr uint32_t SIXTY_NORMAL  = (60.0 / 360.0) * UINT32_MAX;
constexpr float    U8MAX = (float)UINT8_MAX;

namespace {


void convert_hsb_to_rgb(uint32_t hue, uint32_t saturation, uint32_t brightness, uint8_t* red, uint8_t* green, uint8_t* blue) {
  float hue_l = (float)hue;
  float saturation_l = ((float)saturation / (float)UINT32_MAX);
  float brightness_l = ((float)brightness / (float)UINT32_MAX);

  float chroma = saturation_l * brightness_l;
  float hue_prime = fmod((hue_l / 60.0f), 6 );
  float x = chroma * (1.0f - fabs(fmod(hue_prime, 2) - 1.0f));
  float m = brightness_l - chroma;

  float red_l = 0.0f;
  float green_l = 0.0f;
  float blue_l = 0.0f;

  switch((uint8_t)hue_prime) {
    case 0: red_l = chroma; green_l = x;      blue_l = 0; break;
    case 1: red_l = x;      green_l = chroma; blue_l = 0; break;
    case 2: red_l = 0;      green_l = chroma; blue_l = x; break;
    case 3: red_l = 0;      green_l = x;      blue_l = chroma; break;
    case 4: red_l = x;      green_l = 0;      blue_l = chroma; break;
    case 5: red_l = chroma; green_l = 0;      blue_l = x; break;
    default: red_l = 0;     green_l = 0;      blue_l = 0; break;
  }

  red_l += m;
  green_l += m;
  blue_l += m;

  *red = (uint8_t)(U8MAX * red_l);
  *green = (uint8_t)(U8MAX * green_l);
  *blue = (uint8_t)(U8MAX * blue_l);
}


} // namespace

StatusAnimator::StatusAnimator() : _timer(*this, 10, true) {
  _timer.start();
}

StatusAnimator::~StatusAnimator() {
  systick_delete_delegate(&_timer);
}

void StatusAnimator::timer_event() {
  _curr_time = sys_now();
  uint32_t dt = _curr_time - _prev_time;
  _prev_time = _curr_time;
  _cycle_time += dt;

  if(_cycle_time > _total_cycle_time) {
    _cycle_time -= _total_cycle_time;
  }

  uint32_t cycle_progress = (((uint64_t)(_cycle_time)) << 32) / _total_cycle_time;

  handle_cycle_update(cycle_progress);
}

void StatusAnimator::handle_cycle_update(uint32_t progress) {
  uint32_t hue = 0;
  uint32_t saturation = UINT32_MAX;
  uint32_t brightness = 0;

  // Network connection state modifies the status led color
  if(_server_connected) {
    hue = 120;
  } else if(_network_connected) {
    hue = 210;
  } else {
    hue = 240;
  }

  // Being synced with the transmitter modifies the breathing
  if(_synced) {
    if(progress < HALF_PROGRESS) {
      // Counting up
      brightness = progress * 2;
    } else {
      // Counting down
      brightness = UINT32_MAX - progress * 2;
    }
  } else {
    brightness = UINT32_MAX;
  }

  // Convert the HSB value to RGB ones
  uint8_t r, g, b;
  convert_hsb_to_rgb(hue, saturation, brightness, &r, &g, &b);
  set_status_led(r, g, b);
}
