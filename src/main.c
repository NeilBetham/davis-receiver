#include "gpio.h"
#include "ssi.h"
#include "sysctl.h"
#include "uart.h"
#include "spi.h"
#include "cc1125.h"
#include "utils.h"
#include "status_led.h"

#include <stdlib.h>

int main(void){
  uint32_t reset_reason = SYSCTL_RESC;

  // ================= PLL Setup =======================
  // Bump up the clock speed
  // Set the PLL freq
  SYSCTL_RSCLKCFG &= ~(0x0F0003FF);
  SYSCTL_PLLFREQ1 &= ~(0x00001F1F);
  SYSCTL_PLLFREQ0 &= ~(0x008FFFFF);
  SYSCTL_PLLFREQ0  =   0x0080001E;

  // Wait for the PLL to stabilize
  while (!(SYSCTL_PLLSTAT & 0x1)) {}

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
  PORT_A_AFSEL   = 0x000000FF;
  PORT_A_PCTL    = 0x00000011;
  PORT_A_DEN     = 0x000000FF;

  // Setup UART
  UART0_CTL       = 0x00000000;
  UART0_IBRD      = 0x00000041;
  UART0_FBRD      = 0x00000007;
  UART0_LCRH      = 0x00000060;
  UART0_CC        = 0x00000000;
  UART0_CTL       = 0x00000301;

  // =============== Setup Peripherals  =======================
  init_cc1125();
  init_status_led();

  // Enable interrupts
  GIE()

  // Flash some LEDs to make it known we are alive
  set_status_led(1, 0, 0);
  sleep(10000000);
  set_status_led(0, 1, 0);
  sleep(10000000);
  set_status_led(0, 0, 1);
  sleep(10000000);
  set_status_led(1, 1, 1);
  sleep(10000000);
  set_status_led(0, 0, 0);

  // Enable all interrupts
  CORE_EN0 = 0xFFFFFFFF;
  CORE_EN1 = 0xFFFFFFFF;
  CORE_EN2 = 0xFFFFFFFF;
  CORE_EN3 = 0xFFFFFFFF;

  // ================== Hopping Tables ==================
/*
  // My Channels
  uint32_t channels[] = {
    901862125, 902364460, 902865026, 903367422, 903868415, 904369408,
    904870462, 905372797, 905873790, 906375698, 906876752, 907378172,
    907879653, 908381134, 908883042, 909384950, 909885516, 910387424,
    910888844, 911389898, 911891806, 912393226, 912894280, 913396188,
    913897608, 914399577, 914900570, 915401563, 915903959, 916405379,
    916905945, 917406938, 917909334, 918410815, 918911808, 919413716,
    919915197, 920416617, 920917610, 921418664, 921920572, 922421565,
    922924388, 923424954, 923926435, 924427428, 924929336, 925431244,
    925932725, 926433718, 926935626,
  };

  uint32_t hop_pattern[] ={
    18, 0, 19, 41, 25, 8, 47, 32, 13, 36, 22, 3, 29, 44, 16, 5, 27, 38,
    10, 49, 21, 2, 30, 42, 14, 48, 7, 24, 34, 45, 1, 17, 39, 26, 9, 31,
    50, 37, 12, 20, 33, 4, 43, 28, 15, 35, 6, 40, 11, 23, 46,
  };
*/

  // Bemasher Channels
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

    sleep(1000000);
    set_status_led(0, 0, 0);
  }
}
