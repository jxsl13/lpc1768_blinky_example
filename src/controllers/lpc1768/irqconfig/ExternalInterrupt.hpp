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

    /**
     * @brief Available external interrupts
     */
    enum class ExtIntIndexType : DeviceLPC1768::ValueTypeUnsigned {
        IDX_EINT0 = 0,
        IDX_EINT1 = 1,
        IDX_EINT2 = 2,
        IDX_EINT3 = 3,
    };


    /**
     * @brief How an external interrupt may be triggered
     * Pages 26 through 29 in the UM10360 reference manual
     * 
     *EXTMODEX EXTPOLARX
    *  0       0       Level HIGH triggert den Interrupt
    *  0       1       Level LOW triggert den Interrupt
    *  1       0       Edge Falling triggert den Interrupt
    *  1       1       Edge Rising triggert den Interrupt
    */
    enum class ExtIntTriggerType : DeviceLPC1768::ValueTypeUnsigned {
        TRIGGER_LEVEL_HIGH      = 0,
        TRIGGER_LEVEL_LOW       = 1,
        TRIGGER_EDGE_FALLING    = 2, 
        TRIGGER_EDGE_RISING     = 3,
    };

}
}