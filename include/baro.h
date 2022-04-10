/**
 * @brief Interface for controlling the on board barometere
 */

#pragma once

#include <stdint.h>

#include "registers/helpers.h"

typedef struct PACKED {
  uint8_t command: 4;
  uint8_t zeros: 4;
  uint8_t address;
  uint8_t data;
  uint8_t crc;
} BaroReadCommand;

typedef struct PACKED {
  uint8_t command: 4;
  uint8_t status: 2;
  uint8_t zeros: 2;
  uint8_t data_high;
  uint8_t data_low;
  uint8_t crc;
} BaroReadResponse;

typedef struct PACKED {
  uint8_t command: 4;
  uint8_t zeros: 4;
  uint8_t address;
  uint8_t data;
  uint8_t crc;
} BaroWriteCommand;

typedef struct PACKED {
  uint8_t command: 4;
  uint8_t status: 2;
  uint8_t zeros: 2;
  uint8_t data_high;
  uint8_t data_low;
  uint8_t crc;
} BaroWriteResponse;

typedef struct PACKED {
  uint8_t command: 4;
  uint32_t zeros: 20;
  uint8_t crc;
} BaroSensorReadRequest;

typedef struct PACKED {
  uint8_t command: 4;
  uint8_t status: 2;
  uint16_t data: 12;
  uint8_t zeros: 4;
  uint8_t status: 2;
  uint8_t crc;
} BaroSensorReadResponse;

namespace baro {


void init_barometer();
float read_pressure();


} // namespace baro

