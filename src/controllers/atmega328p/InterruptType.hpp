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
    //IRQn_RESET = 0,        // Not available.
    IRQn_INT0           = 1,
    IRQn_INT1           = 2,
    IRQn_PCINT0         = 3,
    IRQn_PCINT1         = 4,
    IRQn_PCINT2         = 5,
    IRQn_WDT            = 6,
    IRQn_TIMER2_COMPA   = 7,
    IRQn_TIMER2_COMPB   = 8,
    IRQn_TIMER2_OVF     = 9,
    IRQn_TIMER1_CAPT    = 10,
    IRQn_TIMER1_COMPA   = 11,
    IRQn_TIMER1_COMPB   = 12,
    IRQn_TIMER1_OVF     = 13,
    IRQn_TIMER0_COMPA   = 14,
    IRQn_TIMER0_COMPB   = 15,
    IRQn_TIMER0_OVF     = 16,
    IRQn_SPI_STC        = 17,
    IRQn_USART_RXC      = 18,
    IRQn_USART_UDRE     = 19,
    IRQn_USART_TXC      = 20,
    IRQn_ADC            = 21,
    IRQn_EE_READY       = 22,
    IRQn_ANALOG_COMP    = 23,
    IRQn_TWI            = 24,
    IRQn_SPM_READY      = 25
};

} // namespace internal
} // namespace holmes