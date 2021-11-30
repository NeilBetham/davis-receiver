/**
 * @brief Abstraction of MAC
 */

#pragma once

#include "ethernet/phy.h"
#include "ethernet/dma.h"

namespace ethernet {


template <uint32_t BUF_SIZE, uint32_t BUF_COUNT>
class Mac {
public:
  void init();

  void enable();
  void disable();

  void set_mac(const uint8_t mac_addr[6]);
  void get_mac(uint8_t mac_addr[6]);

  void tick();

  Phy& get_phy() { return _phy; };
  dma::DescriptorMgr<BUF_SIZE, BUF_COUNT>& get_desc() { return _desc_mgr; };

private:
  Phy _phy;
  dma::DescriptorMgr<BUF_SIZE, BUF_COUNT> _desc_mgr;
  LinkState _curr_link_state = LinkState::unknown;
};


} // namespace ethernet

#include "impl/ethernet/mac_impl.h"
