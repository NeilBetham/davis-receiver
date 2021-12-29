#include "packet_handler.h"

#include "cc1125.h"
#include "logging.h"

#include <string.h>

void PacketHandler::init() {
  start_rx(_hop_controller.current_hop());
}

void PacketHandler::handle_packet(uint8_t* buffer, uint32_t length) {
  // Check that the length is correct. This _should_ always be true
  log_i("RX Packet Len: {}", length);

  // TODO - CRC Check

  // Pull out RSSI and LQI values from last two bytes
  int8_t rssi = (int8_t)buffer[length - 2];
  uint8_t lqi = buffer[length - 1] & 0x7F;
  uint8_t crc_ok = (buffer[length - 1] & 0x80) > 0;

  // Build the packet struct
  ReceivedPacket packet;
  packet.frequency = _hop_controller.current_hop();
  packet.valid = crc_ok;
  packet.rssi = rssi;
  packet.lqi = lqi;
  memcpy(&packet.data, buffer, 8);
  packet.length = 8;
  packet.station_id = (buffer[0] & 0x07) + 1;
  packet.sensor_id = (buffer[0] & 0xF0) >> 4;
  packet.wind_speed = buffer[1];
  packet.wind_dir = ((((uint16_t)buffer[2]) * 360) / 255);

  // Check that we recevied a valid packet
  if(!packet.valid) {
    start_rx(_hop_controller.current_hop());
    log_i("Packet Invalid");
    return;
  }

  // Push the packet if we have space
  if(_packet_buffer.can_push()) {
    _packet_buffer.push(packet);
  }

  // If we received a valid packet it is time to hop
  log_i("Hopping to: {}", _hop_controller.next_hop());
  start_rx(_hop_controller.hop());
}

bool PacketHandler::packet_waiting() {
  return _packet_buffer.can_pop();
}

ReceivedPacket PacketHandler::get_packet() {
  return _packet_buffer.pop();
}
