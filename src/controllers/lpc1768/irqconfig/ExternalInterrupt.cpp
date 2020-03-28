
#include "ExternalInterrupt.hpp"
#include <utils/BitMacros.hpp>
#include "../LPC17xx.h"


namespace holmes
{
    namespace internal
    {

    /**
     * @brief Available external interrupts
     */
    enum class ExtIntIndexType : DeviceLPC1768::ValueTypeUnsigned {
        IDX_EINT0 = 0,
        IDX_EINT1 = 1,
        IDX_EINT2 = 2,
        IDX_EINT3 = 3,
    };


    /**
     * @brief How an external interrupt may be triggered
     * Pages 26 through 29 in the UM10360 reference manual
     * 
     *EXTMODEX EXTPOLARX
    *  0       0       Level HIGH triggert den Interrupt
    *  0       1       Level LOW triggert den Interrupt
    *  1       0       Edge Falling triggert den Interrupt
    *  1       1       Edge Rising triggert den Interrupt
    */
    enum class ExtIntTriggerType : DeviceLPC1768::ValueTypeUnsigned {
        TRIGGER_LEVEL_HIGH      = 0,
        TRIGGER_LEVEL_LOW       = 1,
        TRIGGER_EDGE_FALLING    = 2, 
        TRIGGER_EDGE_RISING     = 3,
    };

    ExternalInterrupt<DeviceLPC1768, ExtIntIndexType, ExtIntTriggerType>::ExternalInterrupt(IndexType InterruptIndex, TriggerType Trigger)
    {
        m_Index = InterruptIndex;
        m_Trigger = static_cast<ValueType>(Trigger); // in order to do less conversions later on.
    };


    auto ExternalInterrupt<DeviceLPC1768, ExtIntIndexType, ExtIntTriggerType>::getTrigger() const -> TriggerType 
    { 
        return static_cast<TriggerType>(m_Trigger); 
    }

    auto ExternalInterrupt<DeviceLPC1768, ExtIntIndexType, ExtIntTriggerType>::applyTo(ExtIntIndexType InterruptIndex) const -> void
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

    auto ExternalInterrupt<DeviceLPC1768, ExtIntIndexType, ExtIntTriggerType>::apply() const -> void
    {
        applyTo(m_Index);
    };

    auto ExternalInterrupt<DeviceLPC1768, ExtIntIndexType, ExtIntTriggerType>::clearPendingBitOf(ExtIntIndexType InterruptIndex) -> void
    {
       /**
        * clear Pending bit
        * Page 27 in the UM10360 user manual
        */
        ENABLE(LPC_SC->EXTINT, static_cast<ValueType>(InterruptIndex));
    }

    auto ExternalInterrupt<DeviceLPC1768, ExtIntIndexType, ExtIntTriggerType>::clearPendingBit() -> void
    {
        clearPendingBitOf(m_Index);
    };


        
    } // namespace internal
} // namespace holmes
