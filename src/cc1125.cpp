#include "cc1125.h"

#include "registers/gpio.h"
#include "spi.h"
#include "utils.h"
#include "logging.h"

#define SPI_BUFFER_SIZES 130

// Hacky way to handle synchornization
static volatile uint8_t wait = 0;

// ISR Handlers for PORTQ
void port_q_root_handler() {
  wait = 0;
  PORT_Q_ICR |= BIT_0 | BIT_1 | BIT_2 | BIT_3;
}

void GPIOPortQ_ISR(void) { port_q_root_handler(); }
void GPIOPortQ1_ISR(void) { port_q_root_handler(); }
void GPIOPortQ2_ISR(void) { port_q_root_handler(); }
void GPIOPortQ3_ISR(void) { port_q_root_handler(); }

// Populates the header into a pre allocated data buffer returns the number of bytes used
static uint8_t populate_header(uint8_t rw, uint8_t burst, uint16_t addr, uint8_t* buffer) {
  if(addr <= 0x3F) {
    // Not an extended register access
    zero(sizeof(CC1125CommHeader), buffer);
    CC1125CommHeader* buf_head = (CC1125CommHeader*)buffer;
    buf_head->addr = addr;
    buf_head->burst = burst;
    buf_head->rw = rw;
    return sizeof(CC1125CommHeader);
  } else {
    // An extended register access
    zero(sizeof(CC1125CommHeaderExt), buffer);
    CC1125CommHeaderExt* buf_head = (CC1125CommHeaderExt*)buffer;
    buf_head->addr = addr & 0x00FF;
    buf_head->burst = burst;
    buf_head->rw = rw;
    buf_head->two_f = 0x2F;
    return sizeof(CC1125CommHeaderExt);
  }
}

uint8_t send_command(uint8_t strobe){
  CC1125CommHeader header;
  CC1125StatByte   status;
  zero(sizeof(header), &header);
  zero(sizeof(status), &status);

  header.rw = 0;
  header.burst = 0;
  header.addr = strobe;

  spi_transact(0, sizeof(header), (uint8_t*)&header, (uint8_t*)&status);

  return status.state;
}

uint8_t write_register(uint16_t address, uint32_t count, const uint8_t* data){
  if((count + 2) > SPI_BUFFER_SIZES) reset();  // Two bytes are potentially needed to write in the header
  uint8_t write_buffer[SPI_BUFFER_SIZES];
  uint8_t read_buffer[SPI_BUFFER_SIZES];
  zero(sizeof(write_buffer), write_buffer);
  zero(sizeof(read_buffer), read_buffer);

  uint8_t header_bytes = populate_header(0, count > 1, address, write_buffer);
  copy(count, data, &write_buffer[header_bytes]);

  spi_transact(0, header_bytes + count, write_buffer, read_buffer);

  CC1125StatByte* status = (CC1125StatByte*)&read_buffer;

  return status->state;
}

uint8_t read_register(uint16_t address, uint32_t count, uint8_t* data){
  if((count + 2) > SPI_BUFFER_SIZES) reset();  // Two bytes are potentially needed to write in the header
  uint8_t write_buffer[SPI_BUFFER_SIZES];
  uint8_t read_buffer[SPI_BUFFER_SIZES];
  zero(sizeof(write_buffer), write_buffer);
  zero(sizeof(read_buffer), read_buffer);

  uint8_t header_bytes = populate_header(1, count > 1, address, write_buffer);

  spi_transact(0, header_bytes + count, write_buffer, read_buffer);
  copy(count, &read_buffer[header_bytes], data);

  CC1125StatByte* status = (CC1125StatByte*)&read_buffer;

  return status->state;
}

uint8_t read_fifo(uint32_t count, uint8_t* data){
  if((count + 2) > SPI_BUFFER_SIZES) reset();  // Two bytes are potentially needed to write in the header
  uint8_t write_buffer[SPI_BUFFER_SIZES];
  uint8_t read_buffer[SPI_BUFFER_SIZES];
  zero(sizeof(write_buffer), write_buffer);
  zero(sizeof(read_buffer), read_buffer);

  uint8_t header_bytes = populate_header(1, count > 1, CC1125_CMD_FIFO, write_buffer);

  spi_transact(0, header_bytes + count, write_buffer, read_buffer);
  copy(count, &read_buffer[header_bytes], data);

  CC1125StatByte* status = (CC1125StatByte*)&read_buffer;

  return status->state;
}

uint8_t load_config(uint32_t count, const ConfigEntry* entries) {
  for(uint32_t index = 0; index < count; index++) {
    ConfigEntry entry = entries[index];
    write_register(entry.addr, 1, &entry.value);
    uint8_t written_value = 0;
    read_register(entry.addr, 1, &written_value);
    if(written_value != entry.value) {
      log_e("Error writing radio config!");
      reset();
    }
  }
  return 1;
}

uint8_t convert_freq(uint32_t freq, uint8_t* freq0, uint8_t* freq1, uint8_t* freq2) {
  uint64_t vco_freq = freq * 4 * 0x100000000;  // LO Divder is 4 in 820 to 960 MHz range
  vco_freq /= 40000000;          // OSC Freq is 40MHz
  vco_freq *= 65536;             // 2^16 scalar
  vco_freq /= 0x100000000;

  *freq0 = vco_freq & 0x000000FF;
  *freq1 = (vco_freq & 0x0000FF00) >> 8;
  *freq2 = (vco_freq & 0x00FF0000) >> 16;

  return 1;
}

uint8_t init_cc1125() {
  // Setup Port Q
  PORT_Q_DIR    &= ~(BIT_0 | BIT_1 | BIT_2);
  PORT_Q_DIR    |= BIT_3;
  PORT_Q_AFSEL  &= ~(BIT_0 | BIT_1 | BIT_2 | BIT_3);
  PORT_Q_DATA   &= ~(BIT_0 | BIT_1 | BIT_2 | BIT_3);

  // Configure interrupts
  PORT_Q_IM     &= ~(BIT_0 | BIT_1 | BIT_2);
  PORT_Q_IEV    |= BIT_0 | BIT_1 | BIT_2;

  // Enable the GPIO output
  PORT_Q_DEN    |= BIT_0 | BIT_1 | BIT_2 | BIT_3;

  // Pull the CC1125 out of reset
  PORT_Q_DATA   |= BIT_3;

  // Init The SPI Port
  spi_init(0);

  // This register config is specific to RX from a Davis Instruments ISS
  const ConfigEntry defaultSettings[] = {
    {CC1125_IOCFG3,            0x30},
    {CC1125_IOCFG2,            0x30},
    {CC1125_IOCFG1,            0x30},
    {CC1125_IOCFG0,            0x14},
    {CC1125_SYNC1,             0xC9},
    {CC1125_SYNC0,             0x89},
    {CC1125_SYNC_CFG1,         0x0A},
    {CC1125_SYNC_CFG0,         0x08},
    {CC1125_DEVIATION_M,       0x03},
    {CC1125_MODCFG_DEV_E,      0x0C},
    {CC1125_DCFILT_CFG,        0x56},
    {CC1125_PREAMBLE_CFG1,     0x18},
    {CC1125_PREAMBLE_CFG0,     0x3A},
    {CC1125_FREQ_IF_CFG,       0x31},
    {CC1125_IQIC,              0x4E},
    {CC1125_CHAN_BW,           0x03},
    {CC1125_MDMCFG0,           0x05},
    {CC1125_SYMBOL_RATE2,      0x7F},
    {CC1125_SYMBOL_RATE1,      0x75},
    {CC1125_SYMBOL_RATE0,      0x10},
    {CC1125_AGC_CS_THR,        0xEA},
    {CC1125_AGC_CFG1,          0x2D},
    {CC1125_AGC_CFG0,          0x5F},
    {CC1125_FIFO_CFG,          0x00},
    {CC1125_FS_CFG,            0x12},
    {CC1125_PKT_CFG1,          0x03},
    {CC1125_PKT_CFG0,          0x00},
    {CC1125_PA_CFG0,           0x7D},
    {CC1125_PKT_LEN,           0x0A},
    {CC1125_IF_MIX_CFG,        0x00},
    {CC1125_FREQOFF_CFG,       0x24},
    {CC1125_IF_ADC0,           0x05},
    {CC1125_FS_DIG1,           0x00},
    {CC1125_FS_DIG0,           0x5F},
    {CC1125_FS_CAL0,           0x0E},
    {CC1125_FS_DIVTWO,         0x03},
    {CC1125_FS_DSM0,           0x33},
    {CC1125_FS_DVC0,           0x17},
    {CC1125_FS_PFD,            0x50},
    {CC1125_FS_PRE,            0x6E},
    {CC1125_FS_REG_DIV_CML,    0x14},
    {CC1125_FS_SPARE,          0xAC},
    {CC1125_XOSC5,             0x0E},
    {CC1125_XOSC3,             0xC7},
    {CC1125_XOSC1,             0x07},
  };
  uint8_t load_status = load_config(sizeof(defaultSettings) / sizeof(ConfigEntry), defaultSettings);

  // Enable Interrupts
  PORT_Q_IM |= BIT_0 | BIT_1 | BIT_2;

  return load_status;
}

uint8_t start_rx(uint32_t freq) {
  // Convert the freq to the VCO config for the CC1125
  uint8_t freq0;
  uint8_t freq1;
  uint8_t freq2;
  convert_freq(freq, &freq0, &freq1, &freq2);

  // Load the config in the CC1125
  ConfigEntry freqConfig[] = {
    {CC1125_FREQ2, freq2},
    {CC1125_FREQ1, freq1},
    {CC1125_FREQ0, freq0}
  };
  load_config(sizeof(freqConfig) / sizeof(ConfigEntry), freqConfig);

  // Send the RX strobe and verify entry into RX mode
  send_command(CC1125_CMD_SRX);
  uint8_t in_rx = 0;
  while(!in_rx) {
    uint8_t rf_state = send_command(CC1125_CMD_SNOP);
    if(rf_state == 0x01) {
      in_rx = 1;
    } else {
      for(uint32_t index = 0; index < 10000; index++) {}
    }
  }

  return 1;
}

uint8_t wait_for_radio() {
  // Init the synchronization var
  wait = 1;

  // Enable interrupts from the radio
  PORT_Q_IM     |= BIT_0;

  // Wait for the radio to send us an interrupt
  while(wait);

  // Disable interrupts from the radio
  PORT_Q_IM     &= ~(BIT_0);

  return 1;
}

uint8_t get_radio_state() {
  return send_command(CC1125_CMD_SNOP);
}

uint8_t receive_packet(uint32_t freq, uint8_t* buffer) {
  start_rx(freq);
  wait_for_radio();

  // Check how many bytes we recevied
  uint8_t bytes_received = 0;
  read_register(CC1125_NUM_RXBYTES, 1, &bytes_received);
  if(bytes_received != 12) {
    return 0;
  }

  // Read the FIFO and return
  read_fifo(12, buffer);
  return 1;
}
