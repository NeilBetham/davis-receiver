/**
 * @brief Controls the state of the CC1125
 */

#pragma once

#include "hop_controller.h"
#include "timer.h"

#include <stdint.h>

#define PACKET_RX_DWELL_TIME_MS 4000

class RadioController : public TimerDelegate {
public:
  RadioController();

  RadioController(const RadioController& other) = delete;
  RadioController(RadioController&& other) = delete;

  RadioController& operator=(const RadioController& rhs) = delete;
  RadioController& operator=(RadioController&& rhs) = delete;

  void init();

  void good_packet_rx();
  void bad_packet_rx();
  bool should_hop();

  uint32_t current_hop() { return _hop_controller.current_hop(); };
  uint32_t next_hop() { return _hop_controller.next_hop(); };

  void timer_event();


private:
  void handle_hop();

  uint32_t _packet_count = 0;
  uint32_t _bad_packet_count = 0;
  uint32_t _last_packet_rx = 0;

  Timer _dwell_timer;
  HopController _hop_controller;
};
