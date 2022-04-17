#include "crc.h"

CRC::CRC(uint16_t polynomial) {
  _polynomial = polynomial;

  for(uint32_t index = 0; index < 256; index++) {
    uint32_t remainder = index << (8);

    for(uint32_t bit = 0; bit < 8; bit++) {
      if(remainder & 0x8000) {
        remainder = (remainder << 1) ^ _polynomial;
      } else {
        remainder = (remainder << 1);
      }
    }

    _table[index] = remainder;
  }
}

uint16_t CRC::compute(uint8_t* buffer, uint32_t length) {
  uint16_t crc = 0;

  for(uint32_t index = 0; index < length; index++) {
    crc = (crc << 8) ^ _table[(crc >> 8) ^ (uint16_t)(buffer[index])];
  }

  return crc;
}

CRC8::CRC8(uint8_t polynomial) {
  _polynomial = polynomial;

  for(uint32_t index = 0; index < 256; index++) {
    uint32_t remainder = index;

    for(uint32_t bit = 0; bit < 8; bit++) {
      if(remainder & 0x80) {
        remainder = (remainder << 1) ^ _polynomial;
      } else {
        remainder = (remainder << 1);
      }
    }

    _table[index] = remainder;
  }
}

uint8_t CRC8::compute(uint8_t* buffer, uint32_t length) {
  uint8_t crc = 0;

  for(uint32_t index = 0; index < length; index++) {
    crc = _table[crc ^ buffer[index]];
  }

  return crc;
}

