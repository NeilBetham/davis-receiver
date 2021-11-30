/**
 * @brief PHY Interface Class
 */

#pragma once

#include "ethernet/ethernet_defs.h"

namespace ethernet {

enum class LinkSpeed {
  unknown = 0,
  ten,
  hundered,
};

enum class LinkMode {
  unknown = 0,
  simplex,
  duplex,
};

enum class LinkState {
  unknown = 0,
  up,
  down,
};

class Phy {
public:
  Phy() {};
  ~Phy() {};

  void init();

  uint16_t read_reg(uint16_t reg_addr);
  bool write_reg(uint16_t reg_addr, uint16_t value);

  bool wait_for_link();
  LinkSpeed get_link_speed();
  LinkMode get_link_mode();
  LinkState get_link_state();
  uint32_t get_mac_config();

private:
  bool _busy = false; // Using as a really bad mutex, don't do this
};


} // namespace
