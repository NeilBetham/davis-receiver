/**
 * @brief animates the status LED
 */

#pragma once

#include "timer.h"

#include <stdint.h>

class StatusAnimator : public TimerDelegate {
public:
  StatusAnimator();
  ~StatusAnimator();

  void synced_with_transmitter(bool synced) { _synced = synced; };
  void connected_to_network(bool connected) { _network_connected = connected; };
  void connected_to_server(bool connected) { _server_connected = connected; };

  void timer_event();

private:
  void handle_cycle_update(uint32_t progress);

  Timer _timer;

  bool _synced = false;
  bool _network_connected = false;
  bool _server_connected = false;

  uint32_t _cycle_time = 0;
  uint32_t _total_cycle_time = 5000;
  uint32_t _prev_time = 0;
  uint32_t _curr_time = 0;
};
