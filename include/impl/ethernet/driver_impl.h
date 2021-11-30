#include "ethernet/driver.h"

#include "logging.h"

#include "lwip/api.h"
#include "lwip/init.h"
#include "lwip/pbuf.h"
#include "lwip/etharp.h"
#include "lwip/timeouts.h"

namespace ethernet {
namespace {

template <typename T>
void enetif_status_callback(struct netif* netif) {
  log_d("Netif Stat CB");
  ((T*)netif->state)->netif_status_cb(netif);
}

template <typename T>
err_t enetif_init(struct netif* netif) {
  log_d("Netif Init");
  return ((T*)netif->state)->netif_init(netif);
}

template <typename T>
err_t enetif_output(struct netif* netif, struct pbuf* packet) {
  log_d("LWIP -> Net: bytes {}", packet->tot_len);
  return ((T*)netif->state)->netif_output(netif, packet);
}


} // namespace

template <uint32_t BUF_SIZE, uint32_t BUF_COUNT>
void Driver<BUF_SIZE, BUF_COUNT>::init() {
  // Init the MAC
  _mac.init();

  // Init the network stack
  lwip_init();

  // Setup the network interface
  netif_add(&_eth_netif, IP4_ADDR_ANY, IP4_ADDR_ANY, IP4_ADDR_ANY, this, enetif_init<driver_t>, netif_input);
  _eth_netif.name[0] = 'e';
  _eth_netif.name[1] = '0';
  netif_set_status_callback(&_eth_netif, enetif_status_callback<driver_t>);
  netif_set_default(&_eth_netif);
  netif_set_up(&_eth_netif);
}

template <uint32_t BUF_SIZE, uint32_t BUF_COUNT>
void Driver<BUF_SIZE, BUF_COUNT>::start_rx() {
  EMAC_DMAOPMODE |= BIT_1;
  _mac.enable();
}

template <uint32_t BUF_SIZE, uint32_t BUF_COUNT>
void Driver<BUF_SIZE, BUF_COUNT>::stop_rx() {
  _mac.disable();
  EMAC_DMAOPMODE &= ~(BIT_1);
}

template <uint32_t BUF_SIZE, uint32_t BUF_COUNT>
void Driver<BUF_SIZE, BUF_COUNT>::start_tx() {
  EMAC_DMAOPMODE |= BIT_13;
  _mac.enable();
}

template <uint32_t BUF_SIZE, uint32_t BUF_COUNT>
void Driver<BUF_SIZE, BUF_COUNT>::stop_tx() {
  _mac.disable();
  EMAC_DMAOPMODE &= ~(BIT_13);
}

template <uint32_t BUF_SIZE, uint32_t BUF_COUNT>
void Driver<BUF_SIZE, BUF_COUNT>::interrupt_handler() {
  // Check if this is normal or error
  if(EMAC_DMARIS & BIT_16) {
    // Normal - check if TX / RX related
    if(EMAC_DMARIS & BIT_6) {
      // Receive
      EMAC_DMARIS |= BIT_6;
      handle_rx();
    }

    if(EMAC_DMARIS & BIT_0) {
      // Transmit
      EMAC_DMARIS |= BIT_0;
      log_d("TX Compelete");
    }

    if(EMAC_DMARIS & BIT_2) {
      // TX Buff Unavailable
      EMAC_DMARIS |= BIT_2;
      log_d("TX Buff Unvail");
    }

    if(EMAC_DMARIS & BIT_14) {
      // Early Receive Int
      EMAC_DMARIS |= BIT_14;
    }
  } else if(EMAC_DMARIS & BIT_15) {
    log_e("Ethernet error interrupt");
    // There is an abnormal interrupt
    if(EMAC_DMARIS & BIT_1) {
      // TX Stopped
      EMAC_DMARIS |= BIT_1;
      log_e("TX Stopped");
    }

    if(EMAC_DMARIS & BIT_3) {
      // Jabber TO
      EMAC_DMARIS |= BIT_3;
    }

    if(EMAC_DMARIS & BIT_4) {
      // RX FIFO Underflow
      EMAC_DMARIS |= BIT_4;
    }

    if(EMAC_DMARIS & BIT_5) {
      // TX Underflow
      EMAC_DMARIS |= BIT_5;
      log_e("TX Underflow");
    }

    if(EMAC_DMARIS & BIT_7) {
      // RX Buffer Unavailable
      EMAC_DMARIS |= BIT_7;
    }

    if(EMAC_DMARIS & BIT_8) {
      // RX Proc Stopped
      EMAC_DMARIS |= BIT_8;
    }

    if(EMAC_DMARIS & BIT_9) {
      // RX WD TO
      EMAC_DMARIS |= BIT_9;
    }

    if(EMAC_DMARIS & BIT_10) {
      // Early TX Int
      EMAC_DMARIS |= BIT_10;
    }

    if(EMAC_DMARIS & BIT_13) {
      // Fatal Bus Error
      EMAC_DMARIS |= BIT_13;
    }
  } else {
    // Some other interrupt has tripped
  }
}

template <uint32_t BUF_SIZE, uint32_t BUF_COUNT>
void Driver<BUF_SIZE, BUF_COUNT>::tick() {
  _mac.tick();

  if(_mac.get_phy().get_link_state() == LinkState::up && _curr_link_state != LinkState::up) {
    log_i("E0 Link Up");
    start_rx();
    start_tx();
    netif_set_link_up(&_eth_netif);
    _curr_link_state = LinkState::up;
  } else if (_mac.get_phy().get_link_state() == LinkState::down && _curr_link_state != LinkState::down) {
    log_i("E0 Link Down");
    stop_tx();
    stop_rx();
    netif_set_link_down(&_eth_netif);
    _curr_link_state = LinkState::down;
  }

  deliver_rx();
  handle_tx();
  sys_check_timeouts();
}

template <uint32_t BUF_SIZE, uint32_t BUF_COUNT>
bool Driver<BUF_SIZE, BUF_COUNT>::queue_frame(const uint8_t* data, uint32_t count) {
  Buffer<BUF_SIZE> buffer;
  memcpy(buffer.buffer(), data, count);
  buffer.set_size(count);

  if(!_tx_queue.can_push()) { return false; }
  _tx_queue.push(buffer);
  return true;
}


template <uint32_t BUF_SIZE, uint32_t BUF_COUNT>
err_t Driver<BUF_SIZE, BUF_COUNT>::netif_init(struct netif* netif) {
  uint8_t mac_addr[6] = {0};
  _mac.get_mac(mac_addr);
  netif->linkoutput = enetif_output<driver_t>;
  netif->output     = etharp_output;
  netif->mtu        = 1500;
  netif->flags      = NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP | NETIF_FLAG_ETHERNET | NETIF_FLAG_IGMP | NETIF_FLAG_MLD6;

  memcpy(netif->hwaddr, mac_addr, 6);
  netif->hwaddr_len = 6;

  return ERR_OK;
}

template <uint32_t BUF_SIZE, uint32_t BUF_COUNT>
void Driver<BUF_SIZE, BUF_COUNT>::netif_status_cb(struct netif* netif) {
}

template <uint32_t BUF_SIZE, uint32_t BUF_COUNT>
err_t Driver<BUF_SIZE, BUF_COUNT>::netif_output(struct netif* netif, struct pbuf* packet) {
  log_d("Sending packet of length: {}", packet->tot_len);
  Buffer<BUF_SIZE> buf;
  struct pbuf* buf_ptr = packet;
  uint32_t bytes_copied = 0;

  while(buf_ptr != NULL) {
    memcpy(buf.buffer(), packet->payload, packet->len);
    bytes_copied += packet->len;
    buf_ptr = buf_ptr->next;
  }

  queue_frame(buf.buffer(), bytes_copied);
  return ERR_OK;
}

/// @brief Takes packets from DMA and queues them for delivery in main loop
template <uint32_t BUF_SIZE, uint32_t BUF_COUNT>
void Driver<BUF_SIZE, BUF_COUNT>::handle_rx() {
  // Iterate through the RX descriptors to see which have data.
  for(auto& desc : _mac.get_desc().rx_desc()) {
    // Check if the descriptor has been assigned to us
    if(desc.is_owned() == false) { continue; }

    // Copy the DMA buffer to local queue for processing
    // TODO: zerocopy buffering, prolly need some type of pool for this.
    if(_rx_queue.can_push()) {
      log_d("RX Packet Length: {}", desc.frame_length());
      desc.buffer().set_size(desc.frame_length());
      _rx_queue.push(desc.buffer());
    }

    // Disown descriptor
    desc.zero();
    desc.reset_owner();
  }
}

/// @brief Takes queued packets and delivers them to LWIP
template <uint32_t BUF_SIZE, uint32_t BUF_COUNT>
void Driver<BUF_SIZE, BUF_COUNT>::deliver_rx() {
  while(_rx_queue.can_pop()) {
    // Get a queued packet
    auto buffer = _rx_queue.pop();

    // Deliver it to LWIP
    struct pbuf* p = pbuf_alloc(PBUF_RAW, buffer.size(), PBUF_POOL);
    if(p != NULL) {
      pbuf_take(p, buffer.buffer(), buffer.size());
      if(_eth_netif.input(p, &_eth_netif) != ERR_OK) {
        pbuf_free(p);
        log_w("LWIP failed to accept packet");
      }
    } else {
      log_w("Failed to allocate PBUF");
    }
  }
}

template <uint32_t BUF_SIZE, uint32_t BUF_COUNT>
void Driver<BUF_SIZE, BUF_COUNT>::handle_tx() {
  if(!_tx_queue.can_pop()) { return; }

  uint32_t used_desc = 0;
  bool should_start = false;

  if(!_tx_queue.can_pop()) { return; }

  // Load queued messages into TX DMA buffers
  auto& tx_desc = _mac.get_desc().curr_tx_desc();
  auto tx_buf = _tx_queue.pop();
  memcpy(tx_desc.buffer().buffer(), tx_buf.buffer(), tx_buf.size());
  tx_desc.set_frame_length(tx_buf.size());
  tx_desc.prep_for_tx();
  tx_desc.reset_owner();

  log_d("Starting TX DMA");
  tx_dma_poll();
}

template <uint32_t BUF_SIZE, uint32_t BUF_COUNT>
void Driver<BUF_SIZE, BUF_COUNT>::tx_dma_poll() {
  EMAC_TXPOLLD = 0xDEADBEEF;
}


} // namespace ethernet
