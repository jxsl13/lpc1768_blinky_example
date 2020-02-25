#pragma once

#include <stdint.h>


namespace holmes
{
namespace internal
{
/**
 * @brief List of possible Interrupts to be configured.
 */
enum class IRQType : uint8_t
{
    //RESET_IRQn = 0,               // Not available.
    INT0_IRQn           = 1,
    INT1_IRQn           = 2,
    PCINT0_IRQn         = 3,
    PCINT1_IRQn         = 4,
    PCINT2_IRQn         = 5,
    WDT_IRQn            = 6,
    TIMER2_COMPA_IRQn   = 7,
    TIMER2_COMPB_IRQn   = 8,
    TIMER2_OVF_IRQn     = 9,
    TIMER1_CAPT_IRQn    = 10,
    TIMER1_COMPA_IRQn   = 11,
    TIMER1_COMPB_IRQn   = 12,
    TIMER1_OVF_IRQn     = 13,
    TIMER0_COMPA_IRQn   = 14,
    TIMER0_COMPB_IRQn   = 15,
    TIMER0_OVF_IRQn     = 16,
    SPI_STC_IRQn        = 17,
    USART_RXC_IRQn      = 18,
    USART_UDRE_IRQn     = 19,
    USART_TXC_IRQn      = 20,
    ADC_IRQn            = 21,
    EE_READY_IRQn       = 22,
    ANALOG_COMP_IRQn    = 23,
    TWI_IRQn            = 24,
    SPM_READY_IRQn      = 25
};

} // namespace internal
} // namespace holmes