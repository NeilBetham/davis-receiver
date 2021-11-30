#pragma once

#include <stdint.h>

// Simple SPI Interface
uint32_t spi_init(uint8_t ssi_port);
uint32_t spi_transact(uint8_t port, uint32_t size, uint8_t* in_buf, uint8_t* out_buf);
