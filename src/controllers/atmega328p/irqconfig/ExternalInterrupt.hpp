#pragma once

#include <hal/irqconfig/ExternalInterrupt.hpp>
#include "../device.hpp"

namespace holmes
{
namespace internal
{
    // forward declaration
    enum class ExtIntIndexType : DeviceAtMega328p::ValueTypeUnsigned;
    enum class ExtIntTriggerType : DeviceAtMega328p::ValueTypeUnsigned;

    template<>
    class ExternalInterrupt<DeviceAtMega328p, ExtIntIndexType, ExtIntTriggerType>
    {   
        // only used internally
        using ValueType = DeviceAtMega328p::ValueTypeUnsigned;

    public:
        using IndexType = ExtIntIndexType;
        using TriggerType = ExtIntTriggerType;

        ExternalInterrupt(IndexType InterruptIndex, TriggerType Trigger);
        auto getTrigger() const -> TriggerType;
        auto applyTo(IndexType InterruptIndex) const -> void;
        auto apply() const -> void;
        static auto clearPendingBitOf(IndexType InterruptIndex) -> void;
        auto clearPendingBit() -> void;
    
    private:
        IndexType m_Index;
        ValueType m_Trigger;
    };

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

}
}