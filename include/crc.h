/**
 * @brief CCITT-16 CRC Calculator
 */

#pragma once

#include <stdint.h>

class CRC {
public:
  CRC(uint16_t init, uint16_t polynomial, uint16_t residue);

  uint16_t checksum(uint8_t* buffer, uint32_t length);

private:
  uint16_t _table[256];
};
