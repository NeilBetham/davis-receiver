/**
 * @biref uDMA driver
 */

#pragma once

#include "registers/register_helpers.h"

#include <stdint.h>

struct PACKED ChanCTL {
  uint32_t src;
  uint32_t end;
  uint32_t ctl;
  uint32_t : 32;
};

void udma_init();
void udma_set_chan_src(uint16_t chan, uint16_t src);
void udma_set_chan_ctl_primary(uint16_t chan, const ChanCTL& ctl);
void udma_set_chan_ctl_secondary(uint16_t chan, const ChanCTL& ctl);

// Used for more complex scatter gather setups
ChanCTL* udma_ctl_array();
