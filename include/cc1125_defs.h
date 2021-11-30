#pragma once

#include <stdint.h>

#include "registers/register_helpers.h"

// CC1125 Related Defines and structs
// Comm Header
typedef struct PACKED {
  uint8_t addr       : 6;
  uint8_t burst      : 1;
  uint8_t rw         : 1;
} CC1125CommHeader;

// Extended Register Address Comm Header
typedef struct PACKED {
  uint8_t two_f      : 6;
  uint8_t burst      : 1;
  uint8_t rw         : 1;
	uint8_t addr       : 8;
} CC1125CommHeaderExt;

// Status Byte
typedef struct PACKED {
  uint8_t resv       : 4;
  uint8_t state      : 3;
  uint8_t chip_ready : 1;
} CC1125StatByte;

// CC1125 Command Strobes
#define CC1125_CMD_SRES    0x30u
#define CC1125_CMD_SFSTXON 0x31u
#define CC1125_CMD_SXOFF   0x32u
#define CC1125_CMD_SCAL    0x33u
#define CC1125_CMD_SRX     0x34u
#define CC1125_CMD_STX     0x35u
#define CC1125_CMD_SIDLE   0x36u
#define CC1125_CMD_SAFC    0x37u
#define CC1125_CMD_SWOR    0x38u
#define CC1125_CMD_SPWD    0x39u
#define CC1125_CMD_SFRX    0x3Au
#define CC1125_CMD_SFTX    0x3Bu
#define CC1125_CMD_SWORRST 0x3Cu
#define CC1125_CMD_SNOP    0x3Du
#define CC1125_CMD_DMA     0x3Eu
#define CC1125_CMD_FIFO    0x3Fu

// CC1125 Register Addresses
#define CC1125_IOCFG3           0x00u
#define CC1125_IOCFG2           0x01u
#define CC1125_IOCFG1           0x02u
#define CC1125_IOCFG0           0x03u
#define CC1125_SYNC3            0x04u
#define CC1125_SYNC2            0x05u
#define CC1125_SYNC1            0x06u
#define CC1125_SYNC0            0x07u
#define CC1125_SYNC_CFG1        0x08u
#define CC1125_SYNC_CFG0        0x09u
#define CC1125_DEVIATION_M      0x0Au
#define CC1125_MODCFG_DEV_E     0x0Bu
#define CC1125_DCFILT_CFG       0x0Cu
#define CC1125_PREAMBLE_CFG1    0x0Du
#define CC1125_PREAMBLE_CFG0    0x0Eu
#define CC1125_FREQ_IF_CFG      0x0Fu
#define CC1125_IQIC             0x10u
#define CC1125_CHAN_BW          0x11u
#define CC1125_MDMCFG1          0x12u
#define CC1125_MDMCFG0          0x13u
#define CC1125_SYMBOL_RATE2     0x14u
#define CC1125_SYMBOL_RATE1     0x15u
#define CC1125_SYMBOL_RATE0     0x16u
#define CC1125_AGC_REF          0x17u
#define CC1125_AGC_CS_THR       0x18u
#define CC1125_AGC_GAIN_ADJUST  0x19u
#define CC1125_AGC_CFG3         0x1Au
#define CC1125_AGC_CFG2         0x1Bu
#define CC1125_AGC_CFG1         0x1Cu
#define CC1125_AGC_CFG0         0x1Du
#define CC1125_FIFO_CFG         0x1Eu
#define CC1125_DEV_ADDR         0x1Fu
#define CC1125_SETTLING_CFG     0x20u
#define CC1125_FS_CFG           0x21u
#define CC1125_WOR_CFG1         0x22u
#define CC1125_WOR_CFG0         0x23u
#define CC1125_WOR_EVENT0_MSB   0x24u
#define CC1125_WOR_EVENT0_LSB   0x25u
#define CC1125_PKT_CFG2         0x26u
#define CC1125_PKT_CFG1         0x27u
#define CC1125_PKT_CFG0         0x28u
#define CC1125_RFEND_CFG1       0x29u
#define CC1125_RFEND_CFG0       0x2Au
#define CC1125_PA_CFG2          0x2Bu
#define CC1125_PA_CFG1          0x2Cu
#define CC1125_PA_CFG0          0x2Du
#define CC1125_PKT_LEN          0x2Eu
#define CC1125_EXTENDED_ADDRESS 0x2Fu

// CC1125 Extended Register Addresses
#define CC1125_IF_MIX_CFG       0x2F00u
#define CC1125_FREQOFF_CFG      0x2F01u
#define CC1125_TOC_CFG          0x2F02u
#define CC1125_MARC_SPARE       0x2F03u
#define CC1125_ECG_CFG          0x2F04u
#define CC1125_CFM_DATA_CFG     0x2F05u
#define CC1125_EXT_CTRL         0x2F06u
#define CC1125_RCCAL_FINE       0x2F07u
#define CC1125_RCCAL_COARSE     0x2F08u
#define CC1125_RCCAL_OFFSET     0x2F09u
#define CC1125_FREQOFF1         0x2F0Au
#define CC1125_FREQOFF0         0x2F0Bu
#define CC1125_FREQ2            0x2F0Cu
#define CC1125_FREQ1            0x2F0Du
#define CC1125_FREQ0            0x2F0Eu
#define CC1125_IF_ADC2          0x2F0Fu
#define CC1125_IF_ADC1          0x2F10u
#define CC1125_IF_ADC0          0x2F11u
#define CC1125_FS_DIG1          0x2F12u
#define CC1125_FS_DIG0          0x2F13u
#define CC1125_FS_CAL3          0x2F14u
#define CC1125_FS_CAL2          0x2F15u
#define CC1125_FS_CAL1          0x2F16u
#define CC1125_FS_CAL0          0x2F17u
#define CC1125_FS_CHP           0x2F18u
#define CC1125_FS_DIVTWO        0x2F19u
#define CC1125_FS_DSM1          0x2F1Au
#define CC1125_FS_DSM0          0x2F1Bu
#define CC1125_FS_DVC1          0x2F1Cu
#define CC1125_FS_DVC0          0x2F1Du
#define CC1125_FS_LBI           0x2F1Eu
#define CC1125_FS_PFD           0x2F1Fu
#define CC1125_FS_PRE           0x2F20u
#define CC1125_FS_REG_DIV_CML   0x2F21u
#define CC1125_FS_SPARE         0x2F22u
#define CC1125_FS_VCO4          0x2F23u
#define CC1125_FS_VCO3          0x2F24u
#define CC1125_FS_VCO2          0x2F25u
#define CC1125_FS_VCO1          0x2F26u
#define CC1125_FS_VCO0          0x2F27u
#define CC1125_GBIAS6           0x2F28u
#define CC1125_GBIAS5           0x2F29u
#define CC1125_GBIAS4           0x2F2Au
#define CC1125_GBIAS3           0x2F2Bu
#define CC1125_GBIAS2           0x2F2Cu
#define CC1125_GBIAS1           0x2F2Du
#define CC1125_GBIAS0           0x2F2Eu
#define CC1125_IFAMP            0x2F2Fu
#define CC1125_LNA              0x2F30u
#define CC1125_RXMIX            0x2F31u
#define CC1125_XOSC5            0x2F32u
#define CC1125_XOSC4            0x2F33u
#define CC1125_XOSC3            0x2F34u
#define CC1125_XOSC2            0x2F35u
#define CC1125_XOSC1            0x2F36u
#define CC1125_XOSC0            0x2F37u
#define CC1125_ANALOG_SPARE     0x2F38u
#define CC1125_PA_CFG3          0x2F39u
#define CC1125_WOR_TIME1        0x2F64u
#define CC1125_WOR_TIME0        0x2F65u
#define CC1125_WOR_CAPTURE1     0x2F66u
#define CC1125_WOR_CAPTURE0     0x2F67u
#define CC1125_BIST             0x2F68u
#define CC1125_DCFILTOFFSET_I1  0x2F69u
#define CC1125_DCFILTOFFSET_I0  0x2F6Au
#define CC1125_DCFILTOFFSET_Q1  0x2F6Bu
#define CC1125_DCFILTOFFSET_Q0  0x2F6Cu
#define CC1125_IQIE_I1          0x2F6Du
#define CC1125_IQIE_I0          0x2F6Eu
#define CC1125_IQIE_Q1          0x2F6Fu
#define CC1125_IQIE_Q0          0x2F70u
#define CC1125_RSSI1            0x2F71u
#define CC1125_RSSI0            0x2F72u
#define CC1125_MARCSTATE        0x2F73u
#define CC1125_LQI_VAL          0x2F74u
#define CC1125_PQT_SYNC_ERR     0x2F75u
#define CC1125_DEM_STATUS       0x2F76u
#define CC1125_FREQOFF_EST1     0x2F77u
#define CC1125_FREQOFF_EST0     0x2F78u
#define CC1125_AGC_GAIN3        0x2F79u
#define CC1125_AGC_GAIN2        0x2F7Au
#define CC1125_AGC_GAIN1        0x2F7Bu
#define CC1125_AGC_GAIN0        0x2F7Cu
#define CC1125_CFM_RX_DATA_OUT  0x2F7Du
#define CC1125_CFM_TX_DATA_IN   0x2F7Eu
#define CC1125_ASK_SOFT_RX_DATA 0x2F7Fu
#define CC1125_RNDGEN           0x2F80u
#define CC1125_MAGN2            0x2F81u
#define CC1125_MAGN1            0x2F82u
#define CC1125_MAGN0            0x2F83u
#define CC1125_ANG1             0x2F84u
#define CC1125_ANG0             0x2F85u
#define CC1125_CHFILT_I2        0x2F86u
#define CC1125_CHFILT_I1        0x2F87u
#define CC1125_CHFILT_I0        0x2F88u
#define CC1125_CHFILT_Q2        0x2F89u
#define CC1125_CHFILT_Q1        0x2F8Au
#define CC1125_CHFILT_Q0        0x2F8Bu
#define CC1125_GPIO_STATUS      0x2F8Cu
#define CC1125_FSCAL_CTRL       0x2F8Du
#define CC1125_PHASE_ADJUST     0x2F8Eu
#define CC1125_PARTNUMBER       0x2F8Fu
#define CC1125_PARTVERSION      0x2F90u
#define CC1125_SERIAL_STATUS    0x2F91u
#define CC1125_MODEM_STATUS1    0x2F92u
#define CC1125_MODEM_STATUS0    0x2F93u
#define CC1125_MARC_STATUS1     0x2F94u
#define CC1125_MARC_STATUS0     0x2F95u
#define CC1125_PA_IFAMP_TEST    0x2F96u
#define CC1125_FSRF_TEST        0x2F97u
#define CC1125_PRE_TEST         0x2F98u
#define CC1125_PRE_OVR          0x2F99u
#define CC1125_ADC_TEST         0x2F9Au
#define CC1125_DVC_TEST         0x2F9Bu
#define CC1125_ATEST            0x2F9Cu
#define CC1125_ATEST_LVDS       0x2F9Du
#define CC1125_ATEST_MODE       0x2F9Eu
#define CC1125_XOSC_TEST1       0x2F9Fu
#define CC1125_XOSC_TEST0       0x2FA0u
#define CC1125_RXFIRST          0x2FD2u
#define CC1125_TXFIRST          0x2FD3u
#define CC1125_RXLAST           0x2FD4u
#define CC1125_TXLAST           0x2FD5u
#define CC1125_NUM_TXBYTES      0x2FD6u
#define CC1125_NUM_RXBYTES      0x2FD7u
#define CC1125_FIFO_NUM_TXBYTES 0x2FD8u
#define CC1125_FIFO_NUM_RXBYTES 0x2FD9u


