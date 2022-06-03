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

  // Read the frequency offset
  int32_t new_offset = read_frequency_offset();
  _freq_offset_table[_hop_controller.current_index()] += new_offset;
  log_i("New Freq Offset: {}", new_offset);


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
  int32_t offset = _freq_offset_table[_hop_controller.current_index()];
  if(should_hop()) {
    freq = _hop_controller.hop();
    offset = _freq_offset_table[_hop_controller.current_index()];
    _dwell_timer.start();
  }

  log_i("Hop {} - Freq: {}, Offset: {}", _hop_controller.current_index(), freq, offset);

  start_rx(freq + offset);
}

bool RadioController::synced() {
  if(_bad_packet_count < BAD_PACKET_CONT_LIM && _last_packet_rx != 0) {
    return true;
  }
  return false;
}
