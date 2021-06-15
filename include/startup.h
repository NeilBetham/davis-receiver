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
* Description:	startup header for the TM4C Launchpad board,defines the vector table
    handlers and ISRS,also declares external variables
*/

// +-----------------------------------------------------------------------------------+
// +			        Type Definitions and Macros                                    +
// +-----------------------------------------------------------------------------------+

/*
 * Defines a macro DEFAULT that aliases the function prototype
 * to Default_Handler if the function is not defined
*/
#define DEFAULT __attribute__((weak, alias("Default_Handler")))

/* Defines a type for the ISR's in the vector table */
typedef void (*element_t)(void);

/* Defines a type for the vector table */
typedef union {
    element_t isr;   //all ISRs use this type
    void *stack_top; //pointer to top of the stack
} vector_table_t;


// +-----------------------------------------------------------------------------------+
// +			        Prototypes of Basic Exception Handlers                         +
// +-----------------------------------------------------------------------------------+

//Default Handler,does nothing
void Default_Handler(void);

//System Exception Handlers

void Reset_Handler(void);
DEFAULT void NMI_Handler(void);
DEFAULT void SVC_Handler(void);
DEFAULT void DebugMonitor_Handler(void);
DEFAULT void PendSV_Handler(void);
DEFAULT void SysTick_Handler(void);

//Fault Handlers

DEFAULT void HardFault_Handler(void);
DEFAULT void MemManageFault_Handler(void);
DEFAULT void BusFault_Handler(void);
DEFAULT void UsageFault_Handler(void);

// +-----------------------------------------------------------------------------------+
// +                Prototypes of Interrupt Service Routines                           +
// +-----------------------------------------------------------------------------------+
DEFAULT void ADC0Sequence0_ISR(void);
DEFAULT void ADC0Sequence1_ISR(void);
DEFAULT void ADC0Sequence2_ISR(void);
DEFAULT void ADC0Sequence3_ISR(void);
DEFAULT void ADC1Sequence0_ISR(void);
DEFAULT void ADC1Sequence1_ISR(void);
DEFAULT void ADC1Sequence2_ISR(void);
DEFAULT void ADC1Sequence3_ISR(void);

DEFAULT void AES_ISR(void);

DEFAULT void AnalogComparator0_ISR(void);
DEFAULT void AnalogComparator1_ISR(void);
DEFAULT void AnalogComparator2_ISR(void);

DEFAULT void CAN0_ISR(void);
DEFAULT void CAN1_ISR(void);

DEFAULT void DES_ISR(void);

DEFAULT void EPI0_ISR(void);

DEFAULT void EthernetMac_ISR(void);

DEFAULT void FlashCtrl_ISR(void);

DEFAULT void FPE_ISR(void);

DEFAULT void GPIOPortA_ISR(void);
DEFAULT void GPIOPortB_ISR(void);
DEFAULT void GPIOPortC_ISR(void);
DEFAULT void GPIOPortD_ISR(void);
DEFAULT void GPIOPortE_ISR(void);
DEFAULT void GPIOPortF_ISR(void);
DEFAULT void GPIOPortG_ISR(void);
DEFAULT void GPIOPortH_ISR(void);
DEFAULT void GPIOPortJ_ISR(void);
DEFAULT void GPIOPortK_ISR(void);
DEFAULT void GPIOPortL_ISR(void);
DEFAULT void GPIOPortM_ISR(void);
DEFAULT void GPIOPortN_ISR(void);
DEFAULT void GPIOPortP_ISR(void);
DEFAULT void GPIOPortP0_ISR(void);
DEFAULT void GPIOPortP1_ISR(void);
DEFAULT void GPIOPortP2_ISR(void);
DEFAULT void GPIOPortP3_ISR(void);
DEFAULT void GPIOPortP4_ISR(void);
DEFAULT void GPIOPortP5_ISR(void);
DEFAULT void GPIOPortP6_ISR(void);
DEFAULT void GPIOPortP7_ISR(void);
DEFAULT void GPIOPortQ_ISR(void);
DEFAULT void GPIOPortQ0_ISR(void);
DEFAULT void GPIOPortQ1_ISR(void);
DEFAULT void GPIOPortQ2_ISR(void);
DEFAULT void GPIOPortQ3_ISR(void);
DEFAULT void GPIOPortQ4_ISR(void);
DEFAULT void GPIOPortQ5_ISR(void);
DEFAULT void GPIOPortQ6_ISR(void);
DEFAULT void GPIOPortQ7_ISR(void);
DEFAULT void GPIOPortR_ISR(void);
DEFAULT void GPIOPortS_ISR(void);
DEFAULT void GPIOPortT_ISR(void);

DEFAULT void Hibernation_ISR(void);

DEFAULT void I2C0_ISR(void);
DEFAULT void I2C1_ISR(void);
DEFAULT void I2C2_ISR(void);
DEFAULT void I2C3_ISR(void);
DEFAULT void I2C4_ISR(void);
DEFAULT void I2C5_ISR(void);
DEFAULT void I2C6_ISR(void);
DEFAULT void I2C7_ISR(void);
DEFAULT void I2C8_ISR(void);
DEFAULT void I2C9_ISR(void);

DEFAULT void PWM0Fault_ISR(void);
DEFAULT void PWM0Generator0_ISR(void);
DEFAULT void PWM0Generator1_ISR(void);
DEFAULT void PWM0Generator2_ISR(void);
DEFAULT void PWM0Generator3_ISR(void);
DEFAULT void PWM1Fault_ISR(void);
DEFAULT void PWM1Generator0_ISR(void);
DEFAULT void PWM1Generator1_ISR(void);
DEFAULT void PWM1Generator2_ISR(void);
DEFAULT void PWM1Generator3_ISR(void);

DEFAULT void QEI0_ISR(void);
DEFAULT void QEI1_ISR(void);

DEFAULT void SHA_MD5_ISR(void);

DEFAULT void SSI0_ISR(void);
DEFAULT void SSI1_ISR(void);
DEFAULT void SSI2_ISR(void);
DEFAULT void SSI3_ISR(void);

DEFAULT void SystemCtrl_ISR(void);

DEFAULT void SystemException_ISR(void);

DEFAULT void Tamper_ISR(void);

DEFAULT void Timer0A_ISR(void);
DEFAULT void Timer0B_ISR(void);
DEFAULT void Timer1A_ISR(void);
DEFAULT void Timer1B_ISR(void);
DEFAULT void Timer2A_ISR(void);
DEFAULT void Timer2B_ISR(void);
DEFAULT void Timer3A_ISR(void);
DEFAULT void Timer3B_ISR(void);
DEFAULT void Timer4A_ISR(void);
DEFAULT void Timer4B_ISR(void);
DEFAULT void Timer5A_ISR(void);
DEFAULT void Timer5B_ISR(void);
DEFAULT void Timer6A_ISR(void);
DEFAULT void Timer6B_ISR(void);
DEFAULT void Timer7A_ISR(void);
DEFAULT void Timer7B_ISR(void);

DEFAULT void UART0_ISR(void);
DEFAULT void UART1_ISR(void);
DEFAULT void UART2_ISR(void);
DEFAULT void UART3_ISR(void);
DEFAULT void UART4_ISR(void);
DEFAULT void UART5_ISR(void);
DEFAULT void UART6_ISR(void);
DEFAULT void UART7_ISR(void);

DEFAULT void UDMAError_ISR(void);
DEFAULT void UDMASoftware_ISR(void);

DEFAULT void USB0_ISR(void);

DEFAULT void WatchDogTimer_ISR(void);



// +-----------------------------------------------------------------------------------+
// +					External Variables declaration					               +
// +-----------------------------------------------------------------------------------+

//main() of your program
extern int main(void);

//stack pointer
extern int _stack_ptr;
//.text/code,stored in Flash
extern int __etext;
//.data,copied into RAM on boot
extern int __data_start__;
extern int __data_end__;
//.bss,unitialized variables
extern int __bss_start__;
extern int __bss_end__;

/***************************************** END OF FILE *******************************************/
