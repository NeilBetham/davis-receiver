#include "packet_handler.h"

#include "cc1125.h"
#include "logging.h"

#include <string.h>

void PacketHandler::init() {
  _radio_controller.init();
}

void PacketHandler::handle_packet(uint8_t* buffer, uint32_t length) {
  // Check that the length is correct. This _should_ always be true
  log_d("RX Packet Len: {}", length);

  // CRC Check
  uint16_t crc = _crc.compute(buffer, length - 2);

  // Pull out RSSI and LQI values from last two bytes
  int8_t rssi = (int8_t)buffer[length - 2];
  uint8_t lqi = buffer[length - 1] & 0x7F;
  uint8_t crc_ok = crc == 0;

  // Build the packet struct
  ReceivedPacket packet;
  packet.frequency = _radio_controller.current_hop();
  packet.valid = crc_ok;
  packet.rssi = rssi;
  packet.lqi = lqi;
  memcpy(&packet.data, buffer, 8);
  packet.length = 8;
  packet.station_id = (buffer[0] & 0x07) + 1;
  packet.sensor_id = (buffer[0] & 0xF0) >> 4;

  if(!packet.valid) {
    _radio_controller.bad_packet_rx();
    log_i("Packet Invalid - CRC: {:04X}", crc);
    return;
  } else {
    _radio_controller.good_packet_rx();
  }

  // If we have space parse the packet
  if(_reading_buffer.can_push()) {
    auto reading = parse_packet(packet);
    _reading_buffer.push(reading);
  }
}

bool PacketHandler::reading_waiting() {
  return _reading_buffer.can_pop();
}

Reading PacketHandler::get_reading() {
  return _reading_buffer.pop();
}
