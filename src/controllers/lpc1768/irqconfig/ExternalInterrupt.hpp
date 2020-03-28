#pragma once

#include <hal/irqconfig/ExternalInterrupt.hpp>
#include "../device.hpp"

namespace holmes
{
namespace internal
{

    enum class ExtIntIndexType : DeviceLPC1768::ValueTypeUnsigned;
    enum class ExtIntTriggerType : DeviceLPC1768::ValueTypeUnsigned;

    template<>
    class ExternalInterrupt<DeviceLPC1768, ExtIntIndexType, ExtIntTriggerType>
    {
        // only used internally
        using ValueType =   DeviceLPC1768::ValueTypeUnsigned;
    public:
        using IndexType =   ExtIntIndexType;
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

}
}