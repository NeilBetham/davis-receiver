#include "ethernet/phy.h"

#include "registers/emac.h"
#include "registers/gpio.h"
#include "registers/sysctl.h"

namespace ethernet {

void Phy::init() {
  // Setup Jack LEDS
  PORT_F_AFSEL = 0x000000FF;
  PORT_F_PCTL  = 0x55555555;
  PORT_F_DEN   = 0x000000FF;

  // Enable the PHY clock
  SYSCTL_RCGCEPHY = BIT_0;

  // Reset the PHY
  SYSCTL_SREPHY = BIT_0;
  for(uint8_t index = 0; index < 16; index++);
  SYSCTL_SREPHY = 0x00000000;

  // Wait for the PHY to be ready
  while(!(SYSCTL_PREPHY & BIT_0));
}

uint16_t Phy::read_reg(uint16_t reg_addr) {
  if(_busy) { return 0; }  // TODO: Change this to some proper error handling
  if(EMAC_MIIADDR & BIT_0) { return 0; }  // Access is in progress
  _busy = true;

  // Setup the addr register for our transaction
  EMAC_MIIADDR = (0x1 << 2) | ((reg_addr & 0x1F) << 6) | BIT_0;

  // Wait for the read to finish
  while(EMAC_MIIADDR & BIT_0);
  uint16_t data = EMAC_MIIDATA;
  _busy = false;

  // Return the read value
  return data;
}

bool Phy::write_reg(uint16_t reg_addr, uint16_t value) {
  if(_busy) { return false; }
  if(EMAC_MIIADDR & BIT_0) { return false; }  // Access is in progress
  _busy = true;

  // Setup the addr register for our transaction
  EMAC_MIIDATA &= ~(0x0000FFFF);
  EMAC_MIIDATA |= value;
  EMAC_MIIADDR |= (0x1 << 2) | ((reg_addr & 0x1F) << 6) | BIT_1 | BIT_0;

  // Wait for the write to finish
  while(EMAC_MIIADDR & BIT_0);
  _busy = false;

  return true;
}

bool Phy::wait_for_link () {
  while(get_link_state() != LinkState::up);
  return true;
}

LinkSpeed Phy::get_link_speed() {
  uint16_t phy_stat = read_reg(PHY_REG_EPHYSTS);
  if(phy_stat & BIT_1) {
    return LinkSpeed::ten;
  } else {
    return LinkSpeed::hundered;
  }
}

LinkMode Phy::get_link_mode() {
  uint16_t phy_stat = read_reg(PHY_REG_EPHYSTS);
  if(phy_stat & BIT_2) {
    return LinkMode::duplex;
  } else {
    return LinkMode::simplex;
  }
}

LinkState Phy::get_link_state() {
  uint16_t phy_stat = read_reg(PHY_REG_EPHYSTS);
  if(phy_stat & BIT_0) {
    return LinkState::up;
  } else {
    return LinkState::down;
  }
}

uint32_t Phy::get_mac_config() {
  uint32_t mac_conf = 0;
  if(get_link_mode() == LinkMode::duplex) {
    mac_conf |= BIT_11;
  }

  if(get_link_speed() == LinkSpeed::hundered) {
    mac_conf |= BIT_14;
  }
  return mac_conf;
}

} // namespace ethernet
