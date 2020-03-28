#pragma once

#include <hal/irqconfig/ExternalInterrupt.hpp>
#include "../device.hpp"

namespace holmes
{
namespace internal
{

    enum class ExtIntIndexType : DeviceSTM32F407VG::ValueTypeUnsigned;
    enum class ExtIntTriggerType : DeviceSTM32F407VG::ValueTypeUnsigned;

    template<>
    class ExternalInterrupt<DeviceSTM32F407VG, ExtIntIndexType, ExtIntTriggerType>
    {
        // only used internally
        using ValueType =   DeviceSTM32F407VG::ValueTypeUnsigned;
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




