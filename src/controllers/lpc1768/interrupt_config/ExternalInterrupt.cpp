#include "ExternalInterrupt.hpp"
#include <utils/BitMacros.hpp>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Number of external interrupts
 */
constexpr ValueType size = 4;

void ExternalInterrupt::applyTo(ExternalInterrupt::IndexType InterruptIndex) const
{
    ValueType Index = static_cast<ValueType>(InterruptIndex);

    ValueType Mode = m_Trigger & 0b10;      // get bit at position 1
    ValueType Polar = m_Trigger & 0b01;     // get bit at position 0

    // level or edge triggered
    if(Mode)
        ENABLE(LPC_SC->EXTMODE, Index);
    else
        DISABLE(LPC_SC->EXTMODE, Index);
    
    // low/falling or high/rising
    if (Polar)
        ENABLE(LPC_SC->EXTPOLAR, Index);
    else
        DISABLE(LPC_SC->EXTPOLAR, Index);

}

void ExternalInterrupt::clearPendingBitOf(ExternalInterrupt::IndexType InterruptIndex)
{
    /**
     * clear Pending bit
     * Page 27 in the UM10360 user manual
     */
    ENABLE(LPC_SC->EXTINT, static_cast<ValueType>(InterruptIndex));
}


#ifdef __cplusplus
}
#endif