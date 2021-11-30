#include "ethernet/mac.h"

#include "registers/sysctl.h"
#include "registers/emac.h"

namespace ethernet {

template <uint32_t BUF_SIZE, uint32_t BUF_COUNT>
void Mac<BUF_SIZE, BUF_COUNT>::init() {
  // PHY needs to be inited first other wise shit will get fucked up
  _phy.init();

  // Enable clock to the MAC
  SYSCTL_RCGCEMAC = BIT_0;

  // Reset the MAC and wait for it to be ready
  SYSCTL_SREMAC = BIT_0;
  for(uint8_t index = 0; index < 16; index++);
  SYSCTL_SREMAC &= ~(BIT_0);
  while(!(SYSCTL_PREMAC & BIT_0));

  // Reset the DMA controller and wait for it to be ready
  EMAC_DMABUSMOD |= BIT_0;
  for(uint8_t index = 0; index < 16; index++);
  while(EMAC_DMABUSMOD & BIT_0);

  // Setup some of the DMA Params
  EMAC_DMABUSMOD |= BIT_25 | BIT_23 | BIT_17;
  EMAC_DMAIM |= BIT_16 | BIT_6 | BIT_0;

  // Setup the DMA descriptors
  _desc_mgr.install();

  // Disable a bunch of not needed interrupts
  EMAC_MMCTXIM |= BIT_20 | BIT_15 | BIT_14 | BIT_1;
  EMAC_MMCRXIM |= BIT_17 | BIT_6 | BIT_5 | BIT_0;
  EMAC_IM |= BIT_9 | BIT_3;

  // Enable interrupts from the PHY
  EPHY_IM |= BIT_0;

  // Configure some of the MAC params
  EMAC_ADDR0L    = 0x5379E738;
  EMAC_ADDR0H    = 0x0000B281;
  EMAC_FRAMEFLTR = 0x00000000;
  EMAC_CFG       = 0x30000400;
}

template <uint32_t BUF_SIZE, uint32_t BUF_COUNT>
void Mac<BUF_SIZE, BUF_COUNT>::enable() {
  EMAC_CFG |= BIT_2 | BIT_3;
}

template <uint32_t BUF_SIZE, uint32_t BUF_COUNT>
void Mac<BUF_SIZE, BUF_COUNT>::disable() {
  EMAC_CFG &= ~(BIT_2 | BIT_3);
}


template <uint32_t BUF_SIZE, uint32_t BUF_COUNT>
void Mac<BUF_SIZE, BUF_COUNT>::set_mac(const uint8_t* mac_addr) {
  uint32_t addr_low = 0;
  uint32_t addr_high = 0;

  addr_high |= mac_addr[0] << 8;
  addr_high |= mac_addr[1];
  addr_low  |= mac_addr[2] << 24;
  addr_low  |= mac_addr[3] << 16;
  addr_low  |= mac_addr[4] << 8;
  addr_low  |= mac_addr[5];

  EMAC_ADDR0H = addr_high;
  EMAC_ADDR0L = addr_low;
}

template <uint32_t BUF_SIZE, uint32_t BUF_COUNT>
void Mac<BUF_SIZE, BUF_COUNT>::get_mac(uint8_t* mac_addr) {
  mac_addr[5] = (EMAC_ADDR0H & 0x0000FF00) >> 8;
  mac_addr[4] = EMAC_ADDR0H & 0x000000FF;

  mac_addr[3] = (EMAC_ADDR0L & 0xFF000000) >> 24;
  mac_addr[2] = (EMAC_ADDR0L & 0x00FF0000) >> 16;
  mac_addr[1] = (EMAC_ADDR0L & 0x0000FF00) >> 8;
  mac_addr[0] = EMAC_ADDR0L & 0x000000FF;
}

template <uint32_t BUF_SIZE, uint32_t BUF_COUNT>
void Mac<BUF_SIZE, BUF_COUNT>::tick() {
  if(_phy.get_link_state() == LinkState::up && _curr_link_state != LinkState::up) {
    log_d("MAC link up");
    // Link is up configure the phy
    uint32_t cfg = EMAC_CFG;
    cfg &= ~(BIT_11 | BIT_14);
    EMAC_CFG = cfg |  _phy.get_mac_config();
    EMAC_DMAOPMODE |= BIT_21 | BIT_25;
    _curr_link_state = LinkState::up;
  } else if(_phy.get_link_state() == LinkState::down && _curr_link_state != LinkState::down) {
    log_d("MAC link down");
    // Link is down disable DMA
    EMAC_DMAOPMODE &= ~(BIT_21 | BIT_25);
    _curr_link_state = LinkState::down;
  }
}


} // namespace ethernet
