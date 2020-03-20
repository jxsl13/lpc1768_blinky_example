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
    IRQn_INT0           = 0,
    IRQn_INT1           = 1,
    IRQn_PCINT0         = 2,
    IRQn_PCINT1         = 3,
    IRQn_PCINT2         = 4,
    IRQn_WDT            = 5,
    IRQn_TIMER2_COMPA   = 6,
    IRQn_TIMER2_COMPB   = 7,
    IRQn_TIMER2_OVF     = 8,
    IRQn_TIMER1_CAPT    = 9,
    IRQn_TIMER1_COMPA   = 10,
    IRQn_TIMER1_COMPB   = 11,
    IRQn_TIMER1_OVF     = 12,
    IRQn_TIMER0_COMPA   = 13,
    IRQn_TIMER0_COMPB   = 14,
    IRQn_TIMER0_OVF     = 15,
    IRQn_SPI_STC        = 16,
    IRQn_USART_RXC      = 17,
    IRQn_USART_UDRE     = 18,
    IRQn_USART_TXC      = 19,
    IRQn_ADC            = 20,
    IRQn_EE_READY       = 21,
    IRQn_ANALOG_COMP    = 22,
    IRQn_TWI            = 23,
    IRQn_SPM_READY      = 24
};

} // namespace internal
} // namespace holmes