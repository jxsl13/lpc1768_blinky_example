#include "../stm32f4xx.h"

#include <utils/BitMacros.hpp>

#include "ExternalInterrupt.hpp"


namespace holmes
{
    namespace internal
    {

    ExternalInterrupt<DeviceSTM32F407VG, ExtIntIndexType, ExtIntTriggerType>::ExternalInterrupt(IndexType InterruptIndex, TriggerType Trigger)
    {
        m_Index = InterruptIndex;
        m_Trigger = static_cast<ValueType>(Trigger); // in order to do less conversions later on.
    }


    auto ExternalInterrupt<DeviceSTM32F407VG, ExtIntIndexType, ExtIntTriggerType>::getTrigger() const -> TriggerType 
    { 
        return static_cast<TriggerType>(m_Trigger); 
    }

    auto ExternalInterrupt<DeviceSTM32F407VG, ExtIntIndexType, ExtIntTriggerType>::applyTo(ExtIntIndexType InterruptIndex) const -> void
    {
        ValueType Value = static_cast<ValueType>(InterruptIndex);
    
        bool IsModeInterrupt = ((Value >> 24) & 0x01) == 1;     // [27:24]: = 1 -> Interrupt enabled
        bool IsModeEvent = ((Value >> 28)) == 1;                // [31:28]: = 1 -> Event enabled

        ValueType Index = (Value >> 16) & 0xFF;             // second 8 bit [23:16]
        ValueType Line = Value & 0xFFFF;                    // last 16 bit [15:0]

        if(IsModeInterrupt)
            ENABLE(EXTI->IMR, Index);  // InterruptMaskRegister -> Trigger interrupt/Callback
        
        if(IsModeEvent)
            ENABLE(EXTI->EMR, Index);  // EventMaskRegister     -> Triggers event/no callback

        ValueType FallingEdge   = m_Trigger & 0b01;
        ValueType RisingEdge    = m_Trigger & 0b10;

        if (FallingEdge)
        {
            ENABLE(EXTI->FTSR, Index);
        }
        else
        {
            DISABLE(EXTI->FTSR, Index);
        }

        if (RisingEdge)
        {
            ENABLE(EXTI->RTSR, Index);
        }
        else
        {
            DISABLE(EXTI->RTSR, Index);
        }

        /** 
         *
         * interrupts above 15 are not defined, events above 15 are defined and might be enabled, but are
         * already wired to specific pins/functions, so we cannot wire them here anymore.
         * See Page 383 in the STM32F407VG Reference Manual
         */
        if(Index >= 16)
            return;
        
        // Enable systen config register clock!!!
        ValueType tmp = RCC->APB2ENR;
        RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

        // configure EXT[X] to be triggered by P[A|B|C|D]X
        SYSCFG->EXTICR[Index / 4] |= Line;  // A, B, C, ...H, I

        RCC->APB2ENR = tmp; // disable clock again.
    }

    auto ExternalInterrupt<DeviceSTM32F407VG, ExtIntIndexType, ExtIntTriggerType>::apply() const -> void
    {
        applyTo(m_Index);
    }

    auto ExternalInterrupt<DeviceSTM32F407VG, ExtIntIndexType, ExtIntTriggerType>::clearPendingBitOf(ExtIntIndexType InterruptIndex) -> void
    {
        ValueType Index = (static_cast<ValueType>(InterruptIndex) >> 16) & 0xFF;    // get bits [23:16]
        ENABLE(EXTI->PR, Index);
    }

    auto ExternalInterrupt<DeviceSTM32F407VG, ExtIntIndexType, ExtIntTriggerType>::clearPendingBit() -> void
    {
        clearPendingBitOf(m_Index);
    }
        
    } // namespace internal
} // namespace holmes
