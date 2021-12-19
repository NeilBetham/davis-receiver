#include "udma.h"

#include "registers/sysctl.h"
#include "registers/udma.h"
#include "utils.h"

static __attribute__((aligned(1024))) ChanCTL chan_ctls[64] = {0};


void udma_init() {
  SYSCTL_RCGCDMA |= BIT_0;
  UDMA_CFG |= BIT_0;
  UDMA_CTLBASE = (uint32_t)&chan_ctls;
}

void udma_set_chan_src(uint16_t chan, uint16_t src) {
  uint8_t reg_index = chan / 8;
  uint8_t reg_shift = (chan % 8) * 4;
  if(reg_index == 0) {
    UDMA_CHMAP0 &= ~(0x000F) << reg_shift;
    UDMA_CHMAP0 |= (src & 0x000F) << reg_shift;
  } else if(reg_index == 1) {
    UDMA_CHMAP1 &= ~(0x000F) << reg_shift;
    UDMA_CHMAP1 |= (src & 0x000F) << reg_shift;
  } else if(reg_index == 2) {
    UDMA_CHMAP2 &= ~(0x000F) << reg_shift;
    UDMA_CHMAP2 |= (src & 0x000F) << reg_shift;
  } else if(reg_index == 3) {
    UDMA_CHMAP3 &= ~(0x000F) << reg_shift;
    UDMA_CHMAP3 |= (src & 0x000F) << reg_shift;
  } else {
    reset();
  }
}

void udma_set_chan_ctl_primary(uint16_t chan, const ChanCTL& ctl) {
  chan_ctls[chan] = ctl;
}

void udma_set_chan_ctl_secondary(uint16_t chan, const ChanCTL& ctl) {
  chan_ctls[chan + 32] = ctl;
}

ChanCTL* udma_ctl_array() {
  return chan_ctls;
}
