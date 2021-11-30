// @brief Classes for dealing with ethernet via DMA
#pragma once

#include "ethernet/ethernet_defs.h"
#include "registers/emac.h"
#include "buffer.h"

#include "logging.h"

#include <array>
#include <string.h>

namespace ethernet {
namespace dma {

/// @brief Manages the RX descriptor and it's underlying buffer
template <size_t BUFFER_SIZE>
class RXDescriptor {
public:
  void setup_descriptor(ENetRXDesc* descriptor) {
    *((uint32_t*)&_descriptor) = 0xFFFFFFFF;
    _descriptor = descriptor;
    _descriptor->rdes1 |= BUFFER_SIZE;
    _descriptor->rdes2 = (uint32_t)_buffer.buffer();
    reset_owner();
  };

  bool is_owned() { return !(_descriptor->rdes0 & RX_DES0_OWN); };
  void reset_owner() { _descriptor->rdes0 |= RX_DES0_OWN; };
  void zero() { _buffer.zero(); };
  Buffer<BUFFER_SIZE>& buffer() { return _buffer; };

  uint32_t frame_length() {
    return (_descriptor->rdes0 & 0x3FFF0000) >> 16;
  };

private:
  Buffer<BUFFER_SIZE> _buffer;
  ENetRXDesc* _descriptor = NULL;
};

/// @brief Manages the TX descriptor and it's underlying buffer
template <size_t BUFFER_SIZE>
class TXDescriptor {
public:
  void setup_descriptor(ENetTXDesc* descriptor) {
    _descriptor = descriptor;
    _descriptor->tdes1 |= BUFFER_SIZE;
    _descriptor->tdes2 = (uint32_t)_buffer.buffer();
    reset_owner();
  }

  bool is_owned() { return !(_descriptor->tdes0 & TX_DES0_OWN); }
  void reset_owner() { _descriptor->tdes0 |= TX_DES0_OWN; }
  void zero() {
    _buffer.zero();
    set_frame_length(0);
  }
  Buffer<BUFFER_SIZE>& buffer() { return _buffer; }
  void set_frame_length(uint32_t length) { _descriptor->tdes1 = length & 0x00000FFF; }
  void set_int_on_tx() { _descriptor->tdes0 |= BIT_30; }
  void set_first_frame() { _descriptor->tdes0 |= BIT_28; }
  void set_last_frame() { _descriptor->tdes0 |= BIT_29; }
  void set_disable_crc() { _descriptor->tdes0 |= BIT_27; }
  void prep_for_tx() {
    set_int_on_tx();
    set_first_frame();
    set_last_frame();
//    set_disable_crc();
  }

private:
  Buffer<BUFFER_SIZE> _buffer;
  ENetTXDesc* _descriptor = NULL;
};

/// @brief Manages a list of descriptors and the associated registers in the ENet DMA
template <uint32_t BUFFER_SIZE, uint32_t BUFFER_COUNT>
class DescriptorMgr {
public:
  DescriptorMgr() {
    for(int index = 0; index < BUFFER_COUNT; index++) {
      _rx[index].setup_descriptor(&_rx_descriptors[index]);
      _tx[index].setup_descriptor(&_tx_descriptors[index]);
    }
    _rx_descriptors[BUFFER_COUNT - 1].rdes1 |= RX_DES1_RER;
    _tx_descriptors[BUFFER_COUNT - 1].tdes0 |= TX_DES0_TER;
  };

  void install() {
    EMAC_DMABUSMOD |= BIT_17 | BIT_8 | BIT_7 ;  // Set descriptor length to 8 bytes
    EMAC_RXDLADDR = (uint32_t)&_rx_descriptors;
    EMAC_TXDLADDR = (uint32_t)&_tx_descriptors;
  };

  void remove() {
    EMAC_RXDLADDR = 0;
    EMAC_TXDLADDR = 0;
  };

  void disown_all() {
    for(int index = 0; index < BUFFER_COUNT; index++) {
      _rx[index].reset_owner();
      _tx[index].reset_owner();
    }
  }

  uint32_t desc_count() const {
    return BUFFER_COUNT;
  }

  RXDescriptor<BUFFER_SIZE>& curr_rx_desc() {
    uint32_t desc_ind = (EMAC_HOSRXDESC - (uint32_t)_rx_descriptors) / sizeof(ENetRXDesc);
    return _rx[desc_ind];
  }

  TXDescriptor<BUFFER_SIZE>& curr_tx_desc() {
    uint32_t desc_ind = (EMAC_HOSTXDESC - (uint32_t)_tx_descriptors) / sizeof(ENetTXDesc);
    return _tx[desc_ind];
  }

  std::array<RXDescriptor<BUFFER_SIZE>, BUFFER_COUNT>& rx_desc() { return _rx; };
  std::array<TXDescriptor<BUFFER_SIZE>, BUFFER_COUNT>& tx_desc() { return _tx; };

private:
  ALIGNED ENetRXDesc _rx_descriptors[BUFFER_COUNT];
  ALIGNED ENetTXDesc _tx_descriptors[BUFFER_COUNT];
  std::array<RXDescriptor<BUFFER_SIZE>, BUFFER_COUNT> _rx;
  std::array<TXDescriptor<BUFFER_SIZE>, BUFFER_COUNT> _tx;
};


} // namespace dma
} // namespace ethernet
