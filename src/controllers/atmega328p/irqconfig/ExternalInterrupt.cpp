
#include "ExternalInterrupt.hpp"    // interfaces etc.
#include <utils/BitMacros.hpp>      // Bit manipulation tools
#include <avr/interrupt.h>

namespace holmes
{
    namespace internal
    {

    ExternalInterrupt<DeviceAtMega328p, ExtIntConfigType, ExtIntTriggerType>::ExternalInterrupt(ConfigType ConfigIndex, TriggerType Trigger)
    {
        m_Config = ConfigIndex;
        m_Trigger = static_cast<ValueType>(Trigger); // in order to do less conversions later on.
    };




    auto ExternalInterrupt<DeviceAtMega328p, ExtIntConfigType, ExtIntTriggerType>::applyTo(ExtIntConfigType ConfigIndex) const -> void
    {
        ValueType Index = static_cast<ValueType>(ConfigIndex);
        
        EICRA |= ((m_Trigger & 0b11) << (Index * 2));
    }

    auto ExternalInterrupt<DeviceAtMega328p, ExtIntConfigType, ExtIntTriggerType>::apply() const -> void
    {
        applyTo(m_Config);
    };

    auto ExternalInterrupt<DeviceAtMega328p, ExtIntConfigType, ExtIntTriggerType>::clearPendingBitOf(ExtIntConfigType ConfigIndex) -> void
    {
        // clears either bit 0 or 1 by writing a 1 to it.
        // Page 81 in the manual.
        ENABLE(EIFR, static_cast<ValueType>(ConfigIndex));
    }

    auto ExternalInterrupt<DeviceAtMega328p, ExtIntConfigType, ExtIntTriggerType>::clearPendingBit() -> void
    {
        clearPendingBitOf(m_Config);
    };

    }// namespace internal
} // namespace holmes


