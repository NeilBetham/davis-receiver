#pragma once

#include "gpio.h"

void init_status_led();
void set_status_led(uint8_t red, uint8_t green, uint8_t blue);
