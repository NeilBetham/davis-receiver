#include "baro.h"

#include "crc.h"
#include "registers/sysctl.h"
#include "registers/gpio.h"
#include "registers/ssi.h"
#include "utils.h"

constexpr float BARO_PRESSURE_OFFSET = -1566.6f;
constexpr float BARO_PRESSURE_SCALAR = 46.67;

namespace {


void spi_init() {
  // Enable the clock gates for all the SSI ports
  SYSCTL_RCGCSSI = 0x0000000F;

  // Configure the port A GPIOs
  PORT_A_AFSEL |= BIT_2 | BIT_4 | BIT_5;
  PORT_A_PCTL  |= 0x00000F00;
  PORT_A_DIR   |= BIT_3;
  PORT_A_DATA  |= BIT_3;

  // Enable GPIO outputs
  PORT_A_DEN   |= BIT_2 | BIT_3;// | BIT_4 | BIT_5;

  // Setup the SPI controller
  SSI0_CR0  &= ~(0x0000FFFF);
  SSI0_CC   =    0x00000000;
  SSI0_CPSR =    0x00000078;
  SSI0_CR0  =    0x00000907;
  SSI0_CR1  =    0x00000002;
}

uint32_t spi_transact(uint32_t size, uint8_t* in_buf, uint8_t* out_buf) {
  // Pull chip select low
  PORT_A_DATA &= ~(BIT_3);

  uint32_t bytes_out = 0;  /// Bytes RXed
  uint32_t bytes_in = 0;   /// Bytes TXed

  // Start buffering data out
  while(bytes_out != size && bytes_out != size) {
    // Fill the TX fifo
    while(SSI0_SR & BIT_1 && bytes_in < size) {
      SSI0_DR = in_buf[bytes_in];
      bytes_in++;
    }

    // Empty the RX fifo
    while(SSI0_SR & BIT_2) {
      out_buf[bytes_out] = SSI0_DR;
      bytes_out++;
    }
  }

  // Wait for the SPI interface to not be busy
  while(SSI0_DR & BIT_4);
  usleep(10);

  // Pull chip select high
  PORT_A_DATA |= BIT_3;

  return size;
}


} // namespace


namespace baro {


void init_barometer() {
  // Init the SPI interface
  spi_init();
}

bool read_pressure(float& pressure_value) {
  static CRC8 crc = CRC8(0x2F);

  // First queue up a sensor read request, next response will be the result of this request
  BaroSensorReadRequest read_req{0};
  read_req.command = 0x1;
  read_req.crc = crc.compute((uint8_t*)&read_req, sizeof(read_req) - 1);


  BaroSensorReadResponse read_resp{0};
  spi_transact(sizeof(read_req), (uint8_t*)&read_req, (uint8_t*)&read_resp);

  // We need at least 500 nS between transactions
  usleep(100);

  // Queue up another read request, the response this time will be valid
  spi_transact(sizeof(read_req), (uint8_t*)&read_req, (uint8_t*)&read_resp);

  // Check the crc of the response
  int resp_crc = crc.compute((uint8_t*)&read_resp, sizeof(read_resp));
  if(resp_crc != 0) { return false; }

  // Check the response status
  if(read_resp.status != 0x1) { return false; }

  // Convert the received value
  float kpa = ((float)read_resp.data - BARO_PRESSURE_OFFSET) / BARO_PRESSURE_SCALAR;
  pressure_value = kpa;
  return true;
}


} // namespace baro
