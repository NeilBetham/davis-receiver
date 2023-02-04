#include "radio_controller.h"

#include "cc1125.h"
#include "logging.h"
#include "sys_tick.h"

#define BAD_PACKET_CONT_LIM 5


RadioController::RadioController() :
    _dwell_timer(*this, PACKET_RX_DWELL_TIME_MS, false),
    _sync_timer(*this, SYNC_RX_LIMIT_TIME_MS, false) {}

void RadioController::init() {
  start_rx(_hop_controller.current_hop());
  _sync_timer.start();
}

void RadioController::good_packet_rx() {
  _dwell_timer.stop();
  _sync_timer.stop();
  _packet_count++;
  _bad_packet_count = 0;
  _last_packet_rx = sys_now();

  // Read the frequency offset
  int32_t new_offset = read_frequency_offset();
  _freq_offset_table[_hop_controller.current_index()] += new_offset;
  _freq_bad_pkt_count[_hop_controller.current_index()] = 0;
  log_i("New Freq Offset: {}", new_offset);


  handle_hop();
}

void RadioController::bad_packet_rx() {
  _dwell_timer.stop();
  _bad_packet_count++;
  _freq_bad_pkt_count[_hop_controller.current_index()]++;

  // If we have recevied a bunch of bad packets on this channel
  // in a row then reset the freq offset
  if(_freq_bad_pkt_count[_hop_controller.current_index()] > 3) {
    _freq_offset_table[_hop_controller.current_index()] = 0;
    _freq_bad_pkt_count[_hop_controller.current_index()] = 0;
    log_w("Channel out of sync, resetting offset: {}", _hop_controller.current_index());
  }

  // After a given number of consecutive bad packets we
  // need to wait for the transmitter to come back around
  if(_bad_packet_count == BAD_PACKET_CONT_LIM) {
    log_w("Out of sync with station");
    out_of_sync();
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

void RadioController::timer_event_two() {
  log_w("Timed out waiting for resync, hopping to random channel");
  _hop_controller.hop_random();
  handle_hop();
  _sync_timer.start();
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

void RadioController::out_of_sync() {
  for(uint32_t index = 0; index < 52; index++) {
    _freq_offset_table[index] = 0;
    _freq_bad_pkt_count[index] = 0;
  }
  _sync_timer.start();
  _hop_controller.hop_random();
  handle_hop();
}

bool RadioController::synced() {
  if(_bad_packet_count < BAD_PACKET_CONT_LIM && _last_packet_rx != 0) {
    return true;
  }
  return false;
}
