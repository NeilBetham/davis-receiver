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

// TODO: Collapse this into one class, too lazy for now
class CRC8 {
public:
  CRC8(uint8_t polynomial);

  uint8_t compute(uint8_t* buffer, uint32_t length);

private:
  uint8_t _table[256] = {0};
  uint8_t _polynomial = 0;
};
