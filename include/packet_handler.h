/**
 * @brief Handle packets and verifies their contents
 */

#pragma once

#include <stdint.h>

#include "cc1125.h"
#include "circular_buffer.h"
#include "crc.h"
#include "hop_controller.h"

// TODO - Move data into templated buffer for more well defined access
struct ReceivedPacket {
  uint32_t frequency;
  bool valid;
  int8_t rssi;
  uint8_t lqi;
  uint8_t station_id;
  uint8_t sensor_id;
  uint8_t wind_speed;
  uint16_t wind_dir;
  uint8_t data[8];
  uint8_t length;
};

class PacketHandler : public RadioDelegate {
public:
  PacketHandler() : _crc(0x1021) {};

  void init();
  void handle_packet(uint8_t* buffer, uint32_t length);

	bool packet_waiting();
	ReceivedPacket get_packet();

private:
  CRC _crc;
  CircularBuffer<ReceivedPacket, 10> _packet_buffer;
  HopController _hop_controller;
};
