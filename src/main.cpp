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
#include "sys_tick.h"
#include "circular_buffer.h"
#include "packet_handler.h"
#include "timer.h"
#include "entropy_pool.h"
#include "reading_reporter.h"
#include "status_animator.h"
#include "baro_manager.h"
#include "logging_uart.h"
#include "logging_server.h"

#include "lwip/dhcp.h"
#include "lwip/ip4_addr.h"

#include <stdlib.h>

#include <mbedtls/platform.h>

ethernet::Driver<1550, 10> enet_driver;
UART uart1(UART1_BASE, 115200);
LoggingUart uart_logger(uart1);
PacketHandler packet_handler;
ReadingReporter reading_reporter;
StatusAnimator status_animator;
// BaroManager baro_manager(reading_reporter);

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

  // ==================== Entropy Source Init ================
  EntropyPool::init();


  // =============== Setup Peripherals =======================
  init_status_led();

  systick_init();
  uart1.init();

  // ====================== Setup logging ===================
  LoggingServer logging_server(4321);
  logging_add_sink(&uart_logger);
  logging_add_sink(&logging_server);
  logging_set_log_level(LogLevel::info);
  log_i("Hello World!");

  enet_driver.init();

  register_delegate(&packet_handler);
  init_cc1125();
  init_status_led();


  // Start DHCP
  err_t ret = dhcp_start(&enet_driver.netif());
  if(ret == ERR_MEM) {
    log_e("Failed to start dhcp");
  }

  packet_handler.init();
  reading_reporter.init();
  logging_server.start();

  // Start receiving packets
  while(1) {
    if(packet_handler.reading_waiting()){
      auto reading = packet_handler.get_reading();
      reading_reporter.handle_reading(reading);
      log_i("Davis ISS, RSSI - {}, LQI - {}, Station - {}, Sensor - {}, Data [{:02X}, {:02X}, {:02X}], WindSpeed - {}, WindDir - {}, Name - {}, Value - {}",
        reading.packet.rssi,
        reading.packet.lqi,
        reading.packet.station_id,
        reading.packet.sensor_id,
        reading.raw_data[0],
        reading.raw_data[1],
        reading.raw_data[2],
        reading.wind_speed,
        reading.wind_dir,
        reading_type_string(reading.type),
        reading.value
      );
    }

    // Do some status LED updates
    status_animator.connected_to_network(enet_driver.ip_valid());
    status_animator.connected_to_server(reading_reporter.connected_to_server());
    status_animator.synced_with_transmitter(packet_handler.synced());

    // See if the ethernet driver has shit to do
    enet_driver.tick();

    // See if systick has things it needs to do
    systick_run();

    // Pet the WDT
    WDT0_ICR = 1;
    WFE();
  }
}

