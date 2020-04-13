
#include "ExternalInterrupt.hpp"
#include <utils/BitMacros.hpp>
#include "../LPC17xx.h"


namespace holmes
{
    namespace internal
    {

    ExternalInterrupt<DeviceLPC1768, ExtIntConfigType, ExtIntTriggerType>::ExternalInterrupt(ConfigType InterruptIndex, TriggerType Trigger)
    {
        m_Config = InterruptIndex;
        m_Trigger = static_cast<ValueType>(Trigger); // in order to do less conversions later on.
    };


    auto ExternalInterrupt<DeviceLPC1768, ExtIntConfigType, ExtIntTriggerType>::getTrigger() const -> TriggerType 
    { 
        return static_cast<TriggerType>(m_Trigger); 
    }

    auto ExternalInterrupt<DeviceLPC1768, ExtIntConfigType, ExtIntTriggerType>::applyTo(ExtIntConfigType InterruptIndex) const -> void
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

    auto ExternalInterrupt<DeviceLPC1768, ExtIntConfigType, ExtIntTriggerType>::apply() const -> void
    {
        applyTo(m_Config);
    };

    auto ExternalInterrupt<DeviceLPC1768, ExtIntConfigType, ExtIntTriggerType>::clearPendingBitOf(ExtIntConfigType InterruptIndex) -> void
    {
       /**
        * clear Pending bit
        * Page 27 in the UM10360 user manual
        */
        ENABLE(LPC_SC->EXTINT, static_cast<ValueType>(InterruptIndex));
    }

    auto ExternalInterrupt<DeviceLPC1768, ExtIntConfigType, ExtIntTriggerType>::clearPendingBit() -> void
    {
        clearPendingBitOf(m_Config);
    };


        
    } // namespace internal
} // namespace holmes
