#include "registers/core.h"

// Stack address provided by the linker
extern int _stack_ptr;

// Weak linkage for default handlers
#define DEFAULT __attribute__((weak, alias("Default_Handler")))

// Default Handler that loops indefinitely
extern "C" void Default_Handler() { while(1); };

// System Exception Handlers
void Reset_Handler(void);
DEFAULT void NMI_Handler(void);
DEFAULT void SVC_Handler(void);
DEFAULT void DebugMonitor_Handler(void);
DEFAULT void PendSV_Handler(void);
DEFAULT void SysTick_Handler(void);

// Fault Handlers
DEFAULT void HardFault_Handler(void);
DEFAULT void MemManageFault_Handler(void);
DEFAULT void BusFault_Handler(void);
DEFAULT void UsageFault_Handler(void);

// Peripheral ISRs
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

// Vector Table Element
typedef void (*ISRHandler)(void);
union VectorTableEntry {
    ISRHandler isr;   //all ISRs use this type
    void* stack_top;  //pointer to top of the stack
};

// Build and mark the vector table for the linker
__attribute__((section(".vector_table")))
const VectorTableEntry vectors[] = {
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
    GPIOPortM_ISR,              // 88
    GPIOPortN_ISR,              // 89
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
    0  // 129 Reserved
};

