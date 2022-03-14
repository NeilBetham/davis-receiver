#include "status_led.h"

#include "registers/gpio.h"
#include "registers/pwm.h"
#include "registers/sysctl.h"

void init_status_led() {
  // Enable the clock to the PWM module
  SYSCTL_RCGCPWM |= BIT_0;
  for(uint32_t index = 0; index < 3; index++); // We need to wait at least three cycles before accessing registers

  // Enable the GPIO so we can output the PWM signals
  PORT_R_DIR   = 0x000000FF;
  PORT_R_ODR   = 0x000000FF;
  PORT_R_DEN   = 0x000000FF;
	PORT_R_AFSEL = 0x000000FF;
  PORT_R_PCTL  = 0x66666666;

  // Configure the PWM clocking
  PWM_CC    =  BIT_8 | 0x5;  // Use the /64 divisor

  // Channel 0
  PWM_0CTL  =  0x0;
  PWM_0GENA =  0x0000008C;
  PWM_0LOAD =  0x00000753;
  PWM_0CMPA =  0x00000753;
  PWM_0CTL  |= BIT_0;

  // Channel 1
  PWM_1CTL  =  0x0;
  PWM_1GENA =  0x0000008C;
  PWM_1LOAD =  0x00000753;
  PWM_1CMPA =  0x00000753;
  PWM_1CTL  |= BIT_0;

  // Channel 2
  PWM_2CTL  =  0x0;
  PWM_2GENA =  0x0000008C;
  PWM_2LOAD =  0x00000753;
  PWM_2CMPA =  0x00000753;
  PWM_2CTL  |= BIT_0;

  // Enable the PWM outputs
  PWM_ENABLE = 0x000000FF;
}

void set_status_led(uint8_t red, uint8_t green, uint8_t blue) {
  PWM_2CMPA = ((1875 * red) >> 8);
  PWM_1CMPA = ((1875 * green) >> 8);
  PWM_0CMPA = ((1875 * blue) >> 8);
}
