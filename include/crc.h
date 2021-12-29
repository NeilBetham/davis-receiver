/**
 * @brief CCITT-16 CRC Calculator
 */

#pragma once

#include <stdint.h>

class CRC {
public:
  CRC(uint16_t polynomial);

  uint16_t compute(uint8_t* buffer, uint32_t length);

private:
  uint16_t _table[256] = {0};
  uint16_t _polynomial = 0;
};
