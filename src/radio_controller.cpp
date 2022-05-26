#include "radio_controller.h"

#include "cc1125.h"
#include "logging.h"
#include "sys_tick.h"

#define BAD_PACKET_CONT_LIM 5

RadioController::RadioController() : _dwell_timer(PACKET_RX_DWELL_TIME_MS, false) {
  _dwell_timer.set_delegate(this);
  systick_register_delegate(&_dwell_timer);
}

void RadioController::init() {
  start_rx(_hop_controller.current_hop());
}

void RadioController::good_packet_rx() {
  _dwell_timer.stop();
  _packet_count++;
  _bad_packet_count = 0;
  _last_packet_rx = sys_now();
  handle_hop();
}

void RadioController::bad_packet_rx() {
  _dwell_timer.stop();
  _bad_packet_count++;
  if(_bad_packet_count == BAD_PACKET_CONT_LIM) {
    log_w("Out of sync with station");
  }
  handle_hop();
}

bool RadioController::should_hop() {
  return _bad_packet_count < BAD_PACKET_CONT_LIM;
}

void RadioController::timer_event() {
  log_w("Timed out waiting for packet");
  bad_packet_rx();
}

void RadioController::handle_hop() {
  uint32_t freq = _hop_controller.current_hop();
  if(should_hop()) {
    freq = _hop_controller.hop();
    log_d("Hopping to: {}", freq);
    _dwell_timer.start();
  }
  start_rx(freq);
}

bool RadioController::synced() {
  if(_bad_packet_count < BAD_PACKET_CONT_LIM && _last_packet_rx != 0) {
    return true;
  }
  return false;
}
