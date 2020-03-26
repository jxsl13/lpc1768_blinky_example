#pragma once

#include <hal/InterruptVectorTable.hpp>

#include "device.hpp"
#include "InterruptType.hpp"


namespace holmes
{
    namespace internal
    {
        template <>
        class InterruptVectorTable<DeviceAtMega328p, IRQType>
        {
            /**
             * @brief 
             * Singleton design pattern:
             * https://stackoverflow.com/questions/1008019/c-singleton-design-pattern
             */
        
           private:
            /**
             * @brief Construct a new Interrupt Vector Table object
             */
            InterruptVectorTable();

           public:

            static auto getInstance() -> InterruptVectorTable&;

            auto setISR(IRQType InterruptIndex, void (*Callback)(void)) -> void;

            auto enableIRQ() -> void;

            auto disableIRQ() -> void;

            auto isEnabledIRQ() -> bool;

            auto enableISR(IRQType InterruptIndex) -> void;

            auto disableISR(IRQType InterruptIndex) -> void;

            auto isEnabledISR(IRQType InterruptIndex) -> bool;

            auto triggerISR(IRQType InterruptIndex) -> void;

            auto waitForIRQ() -> void;

        };
    } // namespace internal   
} // namespace holmes

