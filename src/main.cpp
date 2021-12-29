#include "registers/gpio.h"
#include "registers/ssi.h"
#include "registers/sysctl.h"
#include "registers/uart.h"
#include "registers/wdt.h"
#include "ethernet/driver.h"
#include "spi.h"
#include "cc1125.h"
#include "utils.h"
#include "status_led.h"
#include "timing.h"
#include "circular_buffer.h"
#include "packet_handler.h"

#include "lwip/dhcp.h"

#include <stdlib.h>

ethernet::Driver<1550, 10> enet_driver;
UART uart1(UART1_BASE, 115200);
PacketHandler packet_handler;

void EthernetMac_ISR(void) {
  enet_driver.interrupt_handler();
  sleep_int();
}

int main(void){
  uint32_t reset_reason = SYSCTL_RESC;
  // ============== Crystal Init =======================
  // Set the crytsal range to high and clear the power down bit
  SYSCTL_MOSCCTL  |=   0x00000010;
  SYSCTL_MOSCCTL  &= ~(0x0000000C);

  // Wait for the crystal to stabilize
  while(!(SYSCTL_RIS & BIT_8));

  // ================= PLL Setup =======================
  // Bump up the clock speed
  // Set the PLL freq
  SYSCTL_RSCLKCFG &= ~(0x0F0003FF);
  SYSCTL_RSCLKCFG |=   0x03000000;
  SYSCTL_PLLFREQ1 &= ~(0x00001F1F);
  SYSCTL_PLLFREQ0 &= ~(0x008FFFFF);
  SYSCTL_PLLFREQ1 |=   0x00000004;
  SYSCTL_PLLFREQ0 |=   0x00800060;

  // Wait for the PLL to stabilize
  while (!(SYSCTL_PLLSTAT & 0x1))

  // Set the correct memory timings for 120MHz
  SYSCTL_MEMTIM0  &= ~(0x03EF03EF);
  SYSCTL_MEMTIM0  |=   0x01850185;

  // Setup Sysclk to use the PLL
  SYSCTL_RSCLKCFG |=   0x90000003;

  // ================= Peripheral Setup ==================
  // Enable peripheral clock gates
  SYSCTL_RCGCGPIO = 0xFFFFFFFF;
  SYSCTL_RCGCUART = 0x000000FF;

  // ============== Setup UART   ======================
  // Enable UART Clock and associated GPIO port
  PORT_B_AFSEL   |= BIT_0 | BIT_1;
  PORT_B_PCTL    |= 0x00000011;
  PORT_B_DEN     |= BIT_0 | BIT_1;

  // Enable interrupts
  GIE();

  // Enable all interrupts
  CORE_EN0 = 0xFFFFFFFF;
  CORE_EN1 = 0xFFFFFFFF;
  CORE_EN2 = 0xFFFFFFFF;
  CORE_EN3 = 0xFFFFFFFF;

  // =============== Setup Peripherals  =======================
  systick_init();

  uart1.init();

  logging_init(&uart1);
  logging_set_log_level(LogLevel::debug);
  log_i("Hello World!");

  enet_driver.init();

  register_delegate(&packet_handler);
  init_cc1125();
  init_status_led();

  // Flash some LEDs to make it known we are alive
  set_status_led(1, 0, 0);
  sleep(1000);
  set_status_led(0, 1, 0);
  sleep(1000);
  set_status_led(0, 0, 1);
  sleep(1000);
  set_status_led(1, 1, 1);
  sleep(1000);
  set_status_led(0, 0, 0);

  // Start DHCP
  err_t ret = dhcp_start(&enet_driver.netif());
  if(ret == ERR_MEM) {
    log_e("Failed to start dhcp");
  }

  packet_handler.init();

  // Start receiving packets
  while(1) {
    if(packet_handler.packet_waiting()){
      auto packet = packet_handler.get_packet();
      log_i("Main: Packet RX, RSSI - {}, LQI - {}, Station - {}, Sensor - {}, WindSpeed - {}, WindDir - {}",
        packet.rssi,
        packet.lqi,
        packet.station_id,
        packet.sensor_id,
        packet.wind_speed,
        packet.wind_dir
      );
      set_status_led(0, 1, 0);
      sleep(500);
      set_status_led(0, 0, 0);
    }

    // See if the ethernet driver has shit to do
    enet_driver.tick();

    // Pet the WDT
    //WDT0_ICR = 1;
    WFI();
  }
}
