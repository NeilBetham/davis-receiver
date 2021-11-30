/**
 * @brief Definitions and structs relating to TM4C ethernet
 */

#pragma once

#include "registers/register_helpers.h"

#include <stdint.h>

// Ethernet MAC DMA Descriptors
// RX Enhanced
struct PACKED ENetRXDesc {
  uint32_t rdes0;
  uint32_t rdes1;
  uint32_t rdes2;
  uint32_t rdes3;
  uint32_t rdes4;
  uint32_t rdes5;
  uint32_t rdes6;
  uint32_t rdes7;
};

// TX Enhanced
struct PACKED ENetTXDesc {
  uint32_t tdes0;
  uint32_t tdes1;
  uint32_t tdes2;
  uint32_t tdes3;
  uint32_t tdes4;
  uint32_t tdes5;
  uint32_t tdes6;
  uint32_t tdes7;
};

// RX Descriptor Flags and Fields
#define RX_DES0_OWN        BIT_31
#define RX_DES1_RER        BIT_15

// TX Descriptor Flags and Fields
#define TX_DES0_OWN        BIT_31
#define TX_DES0_TER        BIT_21

// PHY Register Addresses
#define PHY_REG_EPHYBMCR    0x0000
#define PHY_REG_EPHYBMSR    0x0001
#define PHY_REG_EPHYID1     0x0002
#define PHY_REG_EPHYID2     0x0003
#define PHY_REG_EPHYANA     0x004
#define PHY_REG_EPHYANLPA   0x0005
#define PHY_REG_EPHYANER    0x0006
#define PHY_REG_EPHYANNPTR  0x0007
#define PHY_REG_EPHYANLNPTR 0x0008
#define PHY_REG_EPHYCFG1    0x0009
#define PHY_REG_EPHYCFG2    0x000A
#define PHY_REG_EPHYCFG3    0x000B
#define PHY_REG_EPHYREGCTL  0x000D
#define PHY_REG_EPHYADDAR   0x000E
#define PHY_REG_EPHYSTS     0x0010
#define PHY_REG_EPHYSCR     0x0011
#define PHY_REG_EPHYMISR1   0x0012
#define PHY_REG_EPHYMISR2   0x0013
#define PHY_REG_EPHYFCSCR   0x0014
#define PHY_REG_EPHYRXERCNT 0x0015
#define PHY_REG_EPHYBISTCR  0x0016
#define PHY_REG_EPHYLEDCR   0x0018
#define PHY_REG_EPHYCTL     0x0019
#define PHY_REG_EPHY10BTSC  0x001A
#define PHY_REG_EPHYBICSR1  0x001B
#define PHY_REG_EPHYBICSR2  0x001C
#define PHY_REG_EPHYCDCR    0x001E
#define PHY_REG_EPHYRCR     0x001F
#define PHY_REG_EPHYLEDCFG  0x0025
