/**
 * @brief Received packet data struct
 */

#pragma once

#include <stdint.h>

struct ReceivedPacket {
  uint32_t frequency;
  bool valid;
  int8_t rssi;
  uint8_t lqi;
  uint8_t station_id;
  uint8_t sensor_id;
  uint8_t data[8];
  uint8_t length;
};

