/**
 * @brief Manages a uart peripheral
 */

#pragma once

#include <stdint.h>

class UART {
public:
  UART(uint32_t base_reg_addr, uint32_t baud_rate) : _base_reg_addr(base_reg_addr), _baud_rate(baud_rate) {};

  void init();

  void send(const uint8_t* buffer, uint32_t length);
  void send(const char* string);

  void recv(uint8_t* buffer, uint32_t length);

private:
  uint32_t _base_reg_addr = 0;
  uint32_t _baud_rate = 0;
};
