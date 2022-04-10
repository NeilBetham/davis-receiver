#include "baro.h"

namespace {


template <typename T>
void calc_crc(T request_struct) {
  uint8_t crc = 0;

  request.crc = crc;
}

template <typename T, typename U>
void spi_transact(T request_struct, U& response_struct) {

}


} // namespace


namespace baro {


void init_barometer() {
  // Init the SPI interface
}

float read_pressure() {

}


} // namespace baro
