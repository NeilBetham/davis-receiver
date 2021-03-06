#include "spi.h"

#include "registers/sysctl.h"
#include "registers/gpio.h"
#include "registers/ssi.h"

uint32_t spi_init(uint8_t port) {
	// Enable the clock gates for the SSI ports
  SYSCTL_RCGCSSI  = 0x0000000F;

	// Configure the relevant GPIOs
  PORT_B_AFSEL |= BIT_5;
  PORT_B_PCTL  |= 0x00F00000;
  PORT_B_DIR   |= BIT_4;
  PORT_B_DATA  |= BIT_4;

  PORT_E_AFSEL |= BIT_4 | BIT_5;
  PORT_E_PCTL  |= 0x00FF0000;

  // Enable all the GPIO outputs
  PORT_B_DEN   |= BIT_4 | BIT_5;
  PORT_E_DEN   |= BIT_4 | BIT_5;

  // Setup SPI bus
  SSI1_CR0  &= ~(0x0000FFFF);
  SSI1_CC    =   0x00000000;
  SSI1_CPSR  =   0x00000078;
  SSI1_CR0   =   0x00000007;
  SSI1_CR1   =   0x00000002;

  return 1;
}

uint32_t spi_transact(uint8_t port, uint32_t size, uint8_t* in_buf, uint8_t* out_buf) {
	// Pull chip select low
	PORT_B_DATA &= ~(BIT_4);

	// Wait for the MISO line to be pulled low by the target
  PORT_E_AFSEL &= ~(BIT_4);
  while((PORT_E_DATA & BIT_5)) {}
  PORT_E_AFSEL |= BIT_4 | BIT_5;

	// Start writing data
	for(int i = 0; i < size; i++) {
		SSI1_DR = in_buf[i];
		while(!(SSI1_SR & BIT_2)) {} // Wait for something in the RX FIFO
		out_buf[i] = SSI1_DR;
	}

  // Pull chip select high
  PORT_B_DATA |= BIT_4;

	return size;
}
