#include "LPC17xx.h"
#include <hal/InterruptHandler.h>
/* externals from startup_ARMCM3.s */
extern uint32_t __Vectors[];              /* vector table ROM  */
 
#define VECTORTABLE_SIZE        (256)     /* size Cortex-M3 vector table      */
#define VECTORTABLE_ALIGNMENT   (0x100ul) /* 16 Cortex + 32 ARMCM3 = 48 words */
                                          /* next power of 2 = 256            */

enum InterruptHandler::InterruptTypes : uint32_t {
    NonMaskableInt_IRQn = 0,    /*!< 2 Non Maskable Interrupt                         */
    MemoryManagement_IRQn,      /*!< 4 Cortex-M3 Memory Management Interrupt          */
    BusFault_IRQn,              /*!< 5 Cortex-M3 Bus Fault Interrupt                  */
    UsageFault_IRQn,            /*!< 6 Cortex-M3 Usage Fault Interrupt                */
    SVCall_IRQn,                /*!< 11 Cortex-M3 SV Call Interrupt                   */
    DebugMonitor_IRQn,          /*!< 12 Cortex-M3 Debug Monitor Interrupt             */
    PendSV_IRQn,                /*!< 14 Cortex-M3 Pend SV Interrupt                   */
    SysTick_IRQn,               /*!< 15 Cortex-M3 System Tick Interrupt               */

/******  LPC17xx Specific Interrupt Numbers *******************************************************/
    WDT_IRQn,                   /*!< Watchdog Timer Interrupt                         */
    TIMER0_IRQn,                /*!< Timer0 Interrupt                                 */
    TIMER1_IRQn,                /*!< Timer1 Interrupt                                 */
    TIMER2_IRQn,                /*!< Timer2 Interrupt                                 */
    TIMER3_IRQn,                /*!< Timer3 Interrupt                                 */
    UART0_IRQn,                 /*!< UART0 Interrupt                                  */
    UART1_IRQn,                 /*!< UART1 Interrupt                                  */
    UART2_IRQn,                 /*!< UART2 Interrupt                                  */
    UART3_IRQn,                 /*!< UART3 Interrupt                                  */
    PWM1_IRQn,                  /*!< PWM1 Interrupt                                   */
    I2C0_IRQn,                  /*!< I2C0 Interrupt                                   */
    I2C1_IRQn,                  /*!< I2C1 Interrupt                                   */
    I2C2_IRQn,                  /*!< I2C2 Interrupt                                   */
    SPI_IRQn,                   /*!< SPI Interrupt                                    */
    SSP0_IRQn,                  /*!< SSP0 Interrupt                                   */
    SSP1_IRQn,                  /*!< SSP1 Interrupt                                   */
    PLL0_IRQn,                  /*!< PLL0 Lock (Main PLL) Interrupt                   */
    RTC_IRQn,                   /*!< Real Time Clock Interrupt                        */
    EINT0_IRQn,                 /*!< External Interrupt 0 Interrupt                   */
    EINT1_IRQn,                 /*!< External Interrupt 1 Interrupt                   */
    EINT2_IRQn,                 /*!< External Interrupt 2 Interrupt                   */
    EINT3_IRQn,                 /*!< External Interrupt 3 Interrupt                   */
    ADC_IRQn,                   /*!< A/D Converter Interrupt                          */
    BOD_IRQn,                   /*!< Brown-Out Detect Interrupt                       */
    USB_IRQn,                   /*!< USB Interrupt                                    */
    CAN_IRQn,                   /*!< CAN Interrupt                                    */
    DMA_IRQn,                   /*!< General Purpose DMA Interrupt                    */
    I2S_IRQn,                   /*!< I2S Interrupt                                    */
    ENET_IRQn,                  /*!< Ethernet Interrupt                               */
    RIT_IRQn,                   /*!< Repetitive Interrupt Timer Interrupt             */
    MCPWM_IRQn,                 /*!< Motor Control PWM Interrupt                      */
    QEI_IRQn,                   /*!< Quadrature Encoder Interface Interrupt           */
    PLL1_IRQn,                  /*!< PLL1 Lock (USB PLL) Interrupt                    */
    USBActivity_IRQn,           /*!< USB Activity Interrupt(For wakeup only)          */
    CANActivity_IRQn            /*!< CAN Activity Interrupt(For wakeup only)          */
} InterruptTypes;
 
InterruptHandler::InterruptHandler()
{
    m_VectorTable.reserve(VECTORTABLE_SIZE);

    uint32_t *vectors = (uint32_t *)SCB->VTOR;

    for (uint32_t i = 0; i < VECTORTABLE_SIZE; i++) 
    {
        m_VectorTable[i] = vectors[i];            /* copy vector table to RAM */
    }

    /* relocate vector table into RAM*/ 
    __disable_irq();

    SCB->VTOR = (uint32_t) m_VectorTable.data();
    
    __DSB();
    __enable_irq();
}