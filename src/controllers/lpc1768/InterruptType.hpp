#pragma once
#include <stdint.h>

namespace holmes
{
namespace internal
{
    
enum class IRQType : uint32_t
{
    /******  Cortex-M3 Processor Exceptions Numbers ***************************************************/

    // Exceptions are not supported, because they do not explicitly define expected or wanted behaviour,
    // but obviously show errors that were made.

    /******  LPC17xx Specific Interrupt Numbers *******************************************************/
    IRQn_WDT = 0,          /*!< Watchdog Timer Interrupt                         */
    IRQn_TIMER0 = 1,       /*!< Timer0 Interrupt                                 */
    IRQn_TIMER1 = 2,       /*!< Timer1 Interrupt                                 */
    IRQn_TIMER2 = 3,       /*!< Timer2 Interrupt                                 */
    IRQn_TIMER3 = 4,       /*!< Timer3 Interrupt                                 */
    IRQn_UART0 = 5,        /*!< UART0 Interrupt                                  */
    IRQn_UART1 = 6,        /*!< UART1 Interrupt                                  */
    IRQn_UART2 = 7,        /*!< UART2 Interrupt                                  */
    IRQn_UART3 = 8,        /*!< UART3 Interrupt                                  */
    IRQn_PWM1 = 9,         /*!< PWM1 Interrupt                                   */
    IRQn_I2C0 = 10,        /*!< I2C0 Interrupt                                   */
    IRQn_I2C1 = 11,        /*!< I2C1 Interrupt                                   */
    IRQn_I2C2 = 12,        /*!< I2C2 Interrupt                                   */
    IRQn_SPI = 13,         /*!< SPI Interrupt                                    */
    IRQn_SSP0 = 14,        /*!< SSP0 Interrupt                                   */
    IRQn_SSP1 = 15,        /*!< SSP1 Interrupt                                   */
    IRQn_PLL0 = 16,        /*!< PLL0 Lock (Main PLL) Interrupt                   */
    IRQn_RTC = 17,         /*!< Real Time Clock Interrupt                        */
    IRQn_EINT0 = 18,       /*!< External Interrupt 0 Interrupt                   */
    IRQn_EINT1 = 19,       /*!< External Interrupt 1 Interrupt                   */
    IRQn_EINT2 = 20,       /*!< External Interrupt 2 Interrupt                   */
    IRQn_EINT3 = 21,       /*!< External Interrupt 3 Interrupt                   */
    IRQn_ADC = 22,         /*!< A/D Converter Interrupt                          */
    IRQn_BOD = 23,         /*!< Brown-Out Detect Interrupt                       */
    IRQn_USB = 24,         /*!< USB Interrupt                                    */
    IRQn_CAN = 25,         /*!< CAN Interrupt                                    */
    IRQn_DMA = 26,         /*!< General Purpose DMA Interrupt                    */
    IRQn_I2S = 27,         /*!< I2S Interrupt                                    */
    IRQn_ENET = 28,        /*!< Ethernet Interrupt                               */
    IRQn_RIT = 29,         /*!< Repetitive Interrupt Timer Interrupt             */
    IRQn_MCPWM = 30,       /*!< Motor Control PWM Interrupt                      */
    IRQn_QEI = 31,         /*!< Quadrature Encoder Interface Interrupt           */
    IRQn_PLL1 = 32,        /*!< PLL1 Lock (USB PLL) Interrupt                    */
    IRQn_USBActivity = 33, /*!< USB Activity Interrupt(For wakeup only)          */
    IRQn_CANActivity = 34  /*!< CAN Activity Interrupt(For wakeup only)          */
};

}
} // namespace holmes