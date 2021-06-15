/*
* Copyright (c) 2018, Shawn D'silva <shawn@shawndsilva.com>
* All rights reserved.
*
*  This file is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This file is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library.  If not, see <http://www.gnu.org/licenses/>.
*
* File:			startup.c
* Author:		Shawn D'silva <https://www.shawndsilva.com>.
* Version:		1.0.0.
* Description:	startup file for the TM4C Launchpad board,defines the vector table,
  and most importantly the Reset_Handler enabling the TM4C to execute the main program when the <RESET>
  button is pressed on board
*/
#include "startup.h"

// +-----------------------------------------------------------------------------------+
// +										            Vector Table                                       +
// +-----------------------------------------------------------------------------------+

__attribute__((section(".vector_table"))) //marks this vector table as a part of the section "".vector_table"
//in the linker script
const vector_table_t vectors[] = {
    {.stack_top = &_stack_ptr}, // 0    Pointer to top of Stack
    Reset_Handler,              // 1    Reset handler is called when the <RESET> button is pressed
    NMI_Handler,                // 2    Non-Maskable Interrupt handler
    HardFault_Handler,          // 3    Hard Fault Handler
    MemManageFault_Handler,     // 4    Memory management fault Handler
    BusFault_Handler,           // 5    Bus Fault Handler
    UsageFault_Handler,         // 6    Usage Fault Handler
    0,                          // 7    Reserved
    0,                          // 8    Reserved
    0,                          // 9    Reserved
    0,                          // 10   Reserved
    SVC_Handler,                // 11   SuperVisor Call Handler
    DebugMonitor_Handler,       // 12   Debug Monitor Handler
    0,                          // 13   Reserved
    PendSV_Handler,             // 14   Pendeable interrupt driven request
    SysTick_Handler,            // 15   SysTick Timer handler
    GPIOPortA_ISR,              // 16   GPIO Port A Interrupt Service Routine
    GPIOPortB_ISR,              // 17   GPIO Port B Interrupt Service Routine
    GPIOPortC_ISR,              // 18   GPIO Port C Interrupt Service Routine
    GPIOPortD_ISR,              // 19   GPIO Port D Interrupt Service Routine
    GPIOPortE_ISR,              // 20   GPIO Port C Interrupt Service Routine
    UART0_ISR,                  // 21   UART 0
    UART1_ISR,                  // 22   UART 1
    SSI0_ISR,                   // 23   SPI 0
    I2C0_ISR,                   // 24
    PWM0Fault_ISR,              // 25
    PWM0Generator0_ISR,         // 26
    PWM0Generator1_ISR,         // 27
    PWM0Generator2_ISR,         // 28
    QEI0_ISR,                   // 29
    ADC0Sequence0_ISR,          // 30
    ADC0Sequence1_ISR,          // 31
    ADC0Sequence2_ISR,          // 32
    ADC0Sequence3_ISR,          // 33
    WatchDogTimer_ISR,          // 34
    Timer0A_ISR,                // 35
    Timer0B_ISR,                // 36
    Timer1A_ISR,                // 37
    Timer1B_ISR,                // 38
    Timer2A_ISR,                // 39
    Timer2B_ISR,                // 40
    AnalogComparator0_ISR,      // 41
    AnalogComparator1_ISR,      // 42
    AnalogComparator2_ISR,      // 43
    SystemCtrl_ISR,             // 44
    FlashCtrl_ISR,              // 45
    GPIOPortF_ISR,              // 46
    GPIOPortG_ISR,              // 47
    GPIOPortH_ISR,              // 48
    UART2_ISR,                  // 49
    SSI1_ISR,                   // 50
    Timer3A_ISR,                // 51
    Timer3B_ISR,                // 52
    I2C1_ISR,                   // 53
    CAN0_ISR,                   // 54
    CAN1_ISR,                   // 55
    EthernetMac_ISR,            // 56
    Hibernation_ISR,            // 57
    USB0_ISR,                   // 58
    PWM0Generator3_ISR,         // 59
    UDMASoftware_ISR,           // 60
    UDMAError_ISR,              // 61
    ADC1Sequence0_ISR,          // 62
    ADC1Sequence1_ISR,          // 63
    ADC1Sequence2_ISR,          // 64
    ADC1Sequence3_ISR,          // 65
    EPI0_ISR,                   // 66
    GPIOPortJ_ISR,              // 67
    GPIOPortK_ISR,              // 68
    GPIOPortL_ISR,              // 69
    SSI2_ISR,                   // 70
    SSI3_ISR,                   // 71
    UART3_ISR,                  // 72
    UART4_ISR,                  // 73
    UART5_ISR,                  // 74
    UART6_ISR,                  // 75
    UART7_ISR,                  // 76
    I2C2_ISR,                   // 77
    I2C3_ISR,                   // 78
    Timer4A_ISR,                // 79
    Timer4B_ISR,                // 80
    Timer5A_ISR,                // 81
    Timer5B_ISR,                // 82
    FPE_ISR,                    // 83
    0, // 84 Reserved
    0, // 85 Reserved
    I2C4_ISR,                   // 86
    I2C5_ISR,                   // 87
    GPIOPortM_ISR,                  // 88
    GPIOPortN_ISR,                  // 89
    0, // 90 Reserved
    Tamper_ISR,                 // 91
    GPIOPortP_ISR,              // 92
    GPIOPortP1_ISR,             // 93
    GPIOPortP2_ISR,             // 94
    GPIOPortP3_ISR,             // 95
    GPIOPortP4_ISR,             // 96
    GPIOPortP5_ISR,             // 97
    GPIOPortP6_ISR,             // 98
    GPIOPortP7_ISR,             // 99
    GPIOPortQ_ISR,              // 100
    GPIOPortQ1_ISR,             // 101
    GPIOPortQ2_ISR,             // 102
    GPIOPortQ3_ISR,             // 103
    GPIOPortQ4_ISR,             // 104
    GPIOPortQ5_ISR,             // 105
    GPIOPortQ6_ISR,             // 106
    GPIOPortQ7_ISR,             // 107
    GPIOPortR_ISR,              // 108
    GPIOPortS_ISR,              // 109
    SHA_MD5_ISR,                // 110
    AES_ISR,                    // 111
    DES_ISR,                    // 112
    0, // 113 Reserved
    Timer6A_ISR,                // 114
    Timer6B_ISR,                // 115
    Timer7A_ISR,                // 116
    Timer7B_ISR,                // 117
    I2C6_ISR,                   // 118
    I2C7_ISR,                   // 119
    0, // 120 Reserved
    0, // 121 Reserved
    0, // 122 Reserved
    0, // 123 Reserved
    0, // 124 Reserved
    I2C8_ISR,                   // 125
    I2C9_ISR,                   // 126
    GPIOPortT_ISR,              // 127
    0, // 128 Reserved
    0 // 129 Reserved
};

// +-----------------------------------------------------------------------------------+
// +                Implementations of Interrupt Service Routines                      +
// +-----------------------------------------------------------------------------------+
void Reset_Handler(void)
{

  int *src, *dest;

  /* copying of the .data values into RAM */

  src = &__etext;
  for (dest = &__data_start__; dest < &__data_end__;)
  {
    *dest++ = *src++;
  }

  /* initializing .bss values to zero*/

  for (dest = &__bss_start__; dest < &__bss_end__;)
  {
    *dest++ = 0;
  }

  /* your program's main() called */
  main();
}

void Default_Handler(void)
{
  while (1)
  {
    //does literally nothing except infinitely loop
  }
}

void exit() {
  while(1) {}
}
/*****************************************END OF FILE*********************************************/
