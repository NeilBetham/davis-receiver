#pragma once

#include "cc1125_defs.h"

#define PACKET_LENGTH 12

struct ConfigEntry {
  uint16_t addr;
  uint8_t value;
};

class RadioDelegate {
public:
  virtual ~RadioDelegate() {};
  virtual void handle_packet(uint8_t* buffer, uint32_t length) = 0;
};


uint8_t send_command(uint8_t strobe);
uint8_t write_register(uint16_t address, uint32_t count, const uint8_t* data);
uint8_t read_register(uint16_t address, uint32_t count, uint8_t* data);
uint8_t read_fifo(uint32_t count, uint8_t* data);
uint8_t load_config(uint32_t count, const ConfigEntry* entries);
uint8_t convert_freq(uint32_t freq, uint8_t* freq0, uint8_t* freq1, uint8_t* freq2);
uint8_t init_cc1125();
uint8_t start_rx(uint32_t freq);
uint8_t get_radio_state();
uint8_t rx_byte_count();
uint8_t receive_packet(uint8_t* buffer);

void register_delegate(RadioDelegate* delegate);
