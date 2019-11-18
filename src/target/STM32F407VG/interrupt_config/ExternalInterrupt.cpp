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
    
    ValueType FallingEdge   = m_Trigger & 0b01;
    ValueType RisingEdge    = m_Trigger & 0b10;

    if (FallingEdge)
    {
        ENABLE(EXTI->FTSR, Index);
    }
    else
    {
        DISABLE(EXTI->FTSR, Index);
    }

    if (RisingEdge)
    {
        ENABLE(EXTI->RTSR, Index);
    }
    else
    {
        DISABLE(EXTI->RTSR, Index);
    }
}

void ExternalInterrupt::retrieveFrom(ExternalInterrupt::IndexType InterruptIndex)
{
    ValueType Index = static_cast<ValueType>(InterruptIndex);
    
    bool FallingEdge   = IS_SET(EXTI->FTSR, Index);
    bool RisingEdge    = IS_SET(EXTI->RTSR, Index);

    if (FallingEdge)
    {
        ENABLE(m_Trigger, 0);
    }
    else
    {
        DISABLE(m_Trigger, 0);
    }

    if (RisingEdge)
    {
        ENABLE(m_Trigger, 1);
    }
    else
    {
        DISABLE(m_Trigger, 1);
    }
}

void ExternalInterrupt::clearPendingBitOf(ExternalInterrupt::IndexType InterruptIndex)
{
    ValueType Index = static_cast<ValueType>(InterruptIndex);
    ENABLE(EXTI->PR, Index);

}


#ifdef __cplusplus
}
#endif