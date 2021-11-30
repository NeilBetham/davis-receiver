/**
 * @brief TM4C129 Ethernet Driver
 */

#pragma once

#include "buffer.h"
#include "circular_buffer.h"
#include "ethernet/ethernet_defs.h"
#include "ethernet/mac.h"

#include "lwip/netif.h"

#include <functional>

namespace ethernet {


template <uint32_t BUF_SIZE, uint32_t BUF_COUNT>
class Driver {
public:
  using driver_t = Driver<BUF_SIZE, BUF_COUNT>;

  Driver() {};
  ~Driver() {};

  void init();

  void start_rx();
  void stop_rx();

  void start_tx();
  void stop_tx();

  // These are hooks for the runtime
  void interrupt_handler();
  void tick();

  struct netif& netif() { return _eth_netif; }

  // Netif Callbacks
  err_t netif_init(struct netif* netif);
  void netif_status_cb(struct netif* netif);
  err_t netif_output(struct netif* netif, struct pbuf* packet);

private:
  Mac<BUF_SIZE, BUF_COUNT> _mac;

  struct netif _eth_netif;

  void handle_rx();
  void deliver_rx();
  void handle_tx();

  void tx_dma_poll();

  void enetif_stat_cb(struct netif* netif) {}

  bool queue_frame(const uint8_t* data, uint32_t count);

  CircularBuffer<Buffer<BUF_SIZE>, BUF_COUNT> _rx_queue;
  CircularBuffer<Buffer<BUF_SIZE>, BUF_COUNT> _tx_queue;

  LinkState _curr_link_state = LinkState::unknown;
};


} // namespace ethernet

#include "impl/ethernet/driver_impl.h"
