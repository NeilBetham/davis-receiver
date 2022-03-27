/**
 * @brief Handle packets and verifies their contents
 */

#pragma once

#include <stdint.h>

#include "cc1125.h"
#include "circular_buffer.h"
#include "crc.h"
#include "radio_controller.h"
#include "received_packet.h"
#include "packet_parser.h"

class PacketHandler : public RadioDelegate {
public:
  PacketHandler() : _crc(0x1021) {};

  void init();
  void handle_packet(uint8_t* buffer, uint32_t length);

	bool reading_waiting();
	Reading get_reading();

  bool synced() { return _radio_controller.synced(); }

private:
  CRC _crc;
  CircularBuffer<Reading, 10> _reading_buffer;
  RadioController _radio_controller;
};
