
#include "ExternalInterrupt.hpp"    // interfaces etc.
#include <utils/BitMacros.hpp>      // Bit manipulation tools


#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Number of external interrupts
 */
constexpr ValueType size = 2;

void ExternalInterrupt::applyTo(ExternalInterrupt::IndexType InterruptIndex) const
{
    ValueType Index = static_cast<ValueType>(InterruptIndex);
    
    EICRA |= ((m_Trigger & 0b11) << (Index * size));

}

void ExternalInterrupt::retrieveFrom(ExternalInterrupt::IndexType InterruptIndex)
{
    ValueType Index = static_cast<ValueType>(InterruptIndex);
   m_Trigger = (EICRA >> (size - (Index * size))) & 0b11;
}

void ExternalInterrupt::clearPendingBitOf(ExternalInterrupt::IndexType InterruptIndex)
{
    // clears either bit 0 or 1 by writing a 1 to it.
    // Page 81 in the manual.
    ENABLE(EIFR, static_cast<ValueType>(InterruptIndex));
}



#ifdef __cplusplus
}
#endif