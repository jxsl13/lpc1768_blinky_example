#pragma once

#include <hal/irqconfig/ExternalInterrupt.hpp>
#include "../device.hpp"

namespace holmes
{
namespace internal
{

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

}
}