#include "uart.h"

#include "registers/uart.h"

#include <limits.h>

constexpr uint64_t sys_clk_frq = 120000000;

void UART::init() {
  // Calculate baud rate reg values
  // Always leave the high speed divider at 16
  uint32_t divisor = sys_clk_frq / (16 * _baud_rate);
  uint32_t fraction = sys_clk_frq % (16 * _baud_rate);
  fraction = (((fraction * 64 * 2) / (_baud_rate * 16)) + 1) / 2;

  // Program registers
  REGISTER(_base_reg_addr + UART_CTL_OFFSET)  =  0x00000000;
  REGISTER(_base_reg_addr + UART_IBRD_OFFSET) =  divisor & 0x0000FFFF;
  REGISTER(_base_reg_addr + UART_FBRD_OFFSET) =  fraction & 0x0000003F;
  REGISTER(_base_reg_addr + UART_LCRH_OFFSET) |= BIT_4 | BIT_5 | BIT_6;
  REGISTER(_base_reg_addr + UART_CC_OFFSET)   =  0x00000000;
  REGISTER(_base_reg_addr + UART_CTL_OFFSET)  |= BIT_0 | BIT_8 | BIT_9;
}

void UART::send(const uint8_t* buffer, uint32_t length) {
  for(uint32_t index = 0; index < length; index++) {
    while(REGISTER(_base_reg_addr + UART_FR_OFFSET) & BIT_5);
    REGISTER(_base_reg_addr + UART_DR_OFFSET) = buffer[index];
  }
}

void UART::send(const char* string) {
  const char* string_index = string;
  uint32_t length = 0;
  while(*(string_index++) != 0) { length++; }
  send((const uint8_t*) string, length);
}

void UART::recv(uint8_t* buffer, uint32_t length) {
  for(uint32_t index = 0; index < length; index++) {
    while(REGISTER(_base_reg_addr + UART_FR_OFFSET) & BIT_4 == 1);
    buffer[index] = REGISTER(_base_reg_addr + UART_DR_OFFSET);
  }
}
