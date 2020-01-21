#pragma once
#include <hal/interrupt_config/ExternalInterrupt.hpp>    // interfaces etc.
#include <utils/BitMacros.hpp>      // Bit manipulation tools


#include <utils/Enum.hpp>
#include "../Interrupt.hpp" // all needed includes, interface, ValueType, etc.

namespace holmes
{
namespace internal
{
enum class IndexType : DeviceAtMega328p::ValueType
{
    EINT0_PD2 = 0,
    EINT1_PD3 = 1,
};

using IndexTypeCheck = EnumCheck<IndexType, IndexType::EINT0_PD2, IndexType::EINT1_PD3>;

enum class TriggerType : DeviceAtMega328p::ValueType
{
    LEVEL_LOW           = 0,
    EDGE_FALLING_RISING = 1,
    EDGE_FALLING        = 2,
    EDGE_RISING         = 3,
};

using TriggerTypeCheck = EnumCheck<TriggerType,
                                   TriggerType::LEVEL_LOW,
                                   TriggerType::EDGE_FALLING_RISING,
                                   TriggerType::EDGE_FALLING,
                                   TriggerType::EDGE_RISING>;

template <>
class ExternalInterrupt<DeviceAtMega328p, DeviceAtMega328p::ValueType, TriggerType, IndexType>
{
    private:
        IndexType m_Index;
        DeviceAtMega328p::ValueType m_Trigger;
    
    public:

    ExternalInterrupt(IndexType InterruptIndex, TriggerType Trigger)
    {
        m_Index = InterruptIndex;
        m_Trigger = static_cast<DeviceAtMega328p::ValueType>(Trigger); // in order to do less conversions later on.
    };

    auto getTrigger() const -> TriggerType
    {
        return static_cast<TriggerType>(m_Trigger);
    }

   
    auto applyTo(IndexType InterruptIndex) const -> void
    {
        if(!IndexTypeCheck::isValue(InterruptIndex))
            return;

        DeviceAtMega328p::ValueType Index = static_cast<DeviceAtMega328p::ValueType>(InterruptIndex);
    
        EICRA |= ((m_Trigger & 0b11) << (Index * 2));
    }

    auto apply() const -> void
    {
        applyTo(m_Index);
    };

    
    auto clearPendingBitOf(IndexType InterruptIndex) -> void
    {
        if(!IndexTypeCheck::isValue(InterruptIndex))
            return;
        // clears bit by writing a 1 to it.
        // Page 81 in the manual.
        ENABLE(EIFR, static_cast<DeviceAtMega328p::ValueType>(InterruptIndex));
    }

    
    auto clearPendingBit() -> void
    {
        clearPendingBitOf(m_Index);
    };

};

} // namespace internal
} // namespace holmes