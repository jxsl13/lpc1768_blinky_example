#pragma once

#include <hal/irqconfig/ExternalInterrupt.hpp>
#include "../device.hpp"

namespace holmes
{
namespace internal
{
    // forward declaration
    enum class ExtIntConfigType : DeviceAtMega328p::ValueTypeUnsigned;
    enum class ExtIntTriggerType : DeviceAtMega328p::ValueTypeUnsigned;

    template<>
    class ExternalInterrupt<DeviceAtMega328p, ExtIntConfigType, ExtIntTriggerType>
    {   
        // only used internally
        using ValueType = DeviceAtMega328p::ValueTypeUnsigned;

    public:
        using ConfigType = ExtIntConfigType;
        using TriggerType = ExtIntTriggerType;

        ExternalInterrupt(ConfigType InterruptIndex, TriggerType Trigger);
        auto getTrigger() const -> TriggerType;
        auto applyTo(ConfigType InterruptIndex) const -> void;
        auto apply() const -> void;
        static auto clearPendingBitOf(ConfigType InterruptIndex) -> void;
        auto clearPendingBit() -> void;
    
    private:
        ConfigType m_Config;
        ValueType m_Trigger;
    };

    enum class ExtIntConfigType : DeviceAtMega328p::ValueTypeUnsigned {
        CFG_INT0_PD2 = 0,
        CFG_INT1_PD3 = 1,
    };

    enum class ExtIntTriggerType : DeviceAtMega328p::ValueTypeUnsigned {
        TRIGGER_LEVEL_LOW               = 0,
        TRIGGER_EDGE_FALLING_RISING     = 1,
        TRIGGER_EDGE_FALLING            = 2, 
        TRIGGER_EDGE_RISING             = 3,
    };

}
}