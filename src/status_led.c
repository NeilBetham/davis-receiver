#include "status_led.h"

void init_status_led() {
  PORT_R_DIR = 0x000000FF;
  PORT_R_ODR = 0x000000FF;
  PORT_R_DEN = 0x000000FF;
	PORT_R_DATA = 0x000000FF;
}

void set_status_led(uint8_t red, uint8_t green, uint8_t blue) {
  uint32_t reg_setting = 0xFFFFFFFF;
  if(red) {
    reg_setting &= ~(BIT_0);
  }

  if(green) {
    reg_setting &= ~(BIT_2);
  }

  if(blue) {
    reg_setting &= ~(BIT_4);
  }

  PORT_R_DATA = reg_setting;
}
