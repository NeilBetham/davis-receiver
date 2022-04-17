/**
 * @brief Parses radio packets and handles conversions
 */

#pragma once

#include "received_packet.h"

#include <stdint.h>
#include <string>

enum class ReadingType : uint8_t {
  unknown = 0,
  super_cap_voltage = 2,
  uv_index = 4,
  rain_rate = 5,
  solar_radiation = 6,
  light = 7,
  temperature = 8,
  wind_speed_gusts = 9,
  humidity = 10,
  rain_clicks = 14,

  /// These last few are not in the wireless protocol but are used to break down each packet
  wind_speed = 100,
  wind_dir = 101,
  barometer = 102
};


struct Reading {
  ReceivedPacket packet;
  ReadingType type;
  uint8_t wind_speed;
  uint16_t wind_dir;
  uint8_t raw_data[3];
  float value;
};


const std::string reading_type_string(ReadingType reading_type);
Reading parse_packet(ReceivedPacket received_packet);
