#pragma once

#include <hal/irqconfig/ExternalInterrupt.hpp>
#include "../device.hpp"

namespace holmes
{
namespace internal
{

    enum class ExtIntConfigType : DeviceLPC1768::ValueTypeUnsigned;
    enum class ExtIntTriggerType : DeviceLPC1768::ValueTypeUnsigned;

    template<>
    class ExternalInterrupt<DeviceLPC1768, ExtIntConfigType, ExtIntTriggerType>
    {
        // only used internally
        using ValueType =   DeviceLPC1768::ValueTypeUnsigned;
    public:
        using ConfigType =   ExtIntConfigType;
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

    /**
     * @brief Available external interrupts
     */
    enum class ExtIntConfigType : DeviceLPC1768::ValueTypeUnsigned {
        CFG_EINT0_P2_10 = 0,
        CFG_EINT1_P2_11 = 1,
        CFG_EINT2_P2_12 = 2,
        CFG_EINT3_P2_13 = 3,
    };


    /**
     * @brief How an external interrupt may be triggered
     * Pages 26 through 29 in the UM10360 reference manual
     * 
     *EXTMODEX EXTPOLARX
    *  0       0       Level LOW triggert den Interrupt
    *  0       1       Level HIGH triggert den Interrupt
    *  1       0       Edge Falling triggert den Interrupt
    *  1       1       Edge Rising triggert den Interrupt
    */
    enum class ExtIntTriggerType : DeviceLPC1768::ValueTypeUnsigned {
        TRIGGER_LEVEL_LOW       = 0,
        TRIGGER_LEVEL_HIGH      = 1,
        TRIGGER_EDGE_FALLING    = 2, 
        TRIGGER_EDGE_RISING     = 3,
    };

}
}