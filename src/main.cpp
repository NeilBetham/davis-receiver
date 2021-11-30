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

#include "lwip/dhcp.h"

#include <stdlib.h>

ethernet::Driver<1550, 10> enet_driver;
UART uart1(UART1_BASE, 115200);

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

  // ================== Hopping Tables ==================
  uint32_t channels[] = {
    902355835, 902857585, 903359336, 903861086, 904362837, 904864587,
    905366338, 905868088, 906369839, 906871589, 907373340, 907875090,
    908376841, 908878591, 909380342, 909882092, 910383843, 910885593,
    911387344, 911889094, 912390845, 912892595, 913394346, 913896096,
    914397847, 914899597, 915401347, 915903098, 916404848, 916906599,
    917408349, 917910100, 918411850, 918913601, 919415351, 919917102,
    920418852, 920920603, 921422353, 921924104, 922425854, 922927605,
    923429355, 923931106, 924432856, 924934607, 925436357, 925938108,
    926439858, 926941609, 927443359
  };

  uint32_t hop_pattern[] = {
    0, 19, 41, 25, 8, 47, 32, 13, 36, 22, 3, 29, 44, 16, 5, 27, 38, 10,
    49, 21, 2, 30, 42, 14, 48, 7, 24, 34, 45, 1, 17, 39, 26, 9, 31, 50,
    37, 12, 20, 33, 4, 43, 28, 15, 35, 6, 40, 11, 23, 46, 18
  };


  uint32_t current_hop = 0;
  const uint32_t max_hop = sizeof(hop_pattern) / sizeof(uint32_t);

  // Start receiving packets
  while(1) {
    log_i("Loop");

    uint32_t hop_freq = channels[hop_pattern[current_hop]];
    uint8_t data_buffer[12] = {0};
    uint8_t should_hop = 0;

    uint8_t packet_recieved = receive_packet(hop_freq, data_buffer);

    if(packet_recieved){
      set_status_led(0, 1, 0);
      should_hop = 1;
    } else {
      set_status_led(1, 0, 0);
    }

    if(should_hop) {
      if(current_hop + 1 >= max_hop) {
        current_hop = 0;
      } else {
        current_hop++;
      }
    }

    sleep(500);
    set_status_led(0, 0, 0);

    // See if the ethernet driver has shit to do
    enet_driver.tick();

    // Pet the WDT
    //WDT0_ICR = 1;
  }
}
