
#include "ExternalInterrupt.hpp"    // interfaces etc.
#include <utils/BitMacros.hpp>      // Bit manipulation tools
#include <avr/interrupt.h>

namespace holmes
{
    namespace internal
    {

        enum class ExtIntIndexType : DeviceAtMega328p::ValueTypeUnsigned {
            IDX_INT0 = 0,
            IDX_INT1 = 1,
        };

        enum class ExtIntTriggerType : DeviceAtMega328p::ValueTypeUnsigned {
            TRIGGER_LEVEL_LOW               = 0,
            TRIGGER_EDGE_FALLING_RISING     = 1,
            TRIGGER_EDGE_FALLING            = 2, 
            TRIGGER_EDGE_RISING             = 3,
        };


    ExternalInterrupt<DeviceAtMega328p, ExtIntIndexType, ExtIntTriggerType>::ExternalInterrupt(IndexType InterruptIndex, TriggerType Trigger)
    {
        m_Index = InterruptIndex;
        m_Trigger = static_cast<ValueType>(Trigger); // in order to do less conversions later on.
    };


    auto ExternalInterrupt<DeviceAtMega328p, ExtIntIndexType, ExtIntTriggerType>::getTrigger() const -> TriggerType 
    { 
        return static_cast<TriggerType>(m_Trigger); 
    }

    auto ExternalInterrupt<DeviceAtMega328p, ExtIntIndexType, ExtIntTriggerType>::applyTo(ExtIntIndexType InterruptIndex) const -> void
    {
        ValueType Index = static_cast<ValueType>(InterruptIndex);
        
        EICRA |= ((m_Trigger & 0b11) << (Index * 2));
    }

    auto ExternalInterrupt<DeviceAtMega328p, ExtIntIndexType, ExtIntTriggerType>::apply() const -> void
    {
        applyTo(m_Index);
    };

    auto ExternalInterrupt<DeviceAtMega328p, ExtIntIndexType, ExtIntTriggerType>::clearPendingBitOf(ExtIntIndexType InterruptIndex) -> void
    {
        // clears either bit 0 or 1 by writing a 1 to it.
        // Page 81 in the manual.
        ENABLE(EIFR, static_cast<ValueType>(InterruptIndex));
    }

    auto ExternalInterrupt<DeviceAtMega328p, ExtIntIndexType, ExtIntTriggerType>::clearPendingBit() -> void
    {
        clearPendingBitOf(m_Index);
    };

    }// namespace internal
} // namespace holmes


