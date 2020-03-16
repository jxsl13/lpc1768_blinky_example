#pragma once

#include <hal/Interrupt.hpp>
#include "device.hpp"
#include "InterruptType.hpp"

namespace holmes
{
    namespace internal
    {
        extern template class InterruptVectorTable<DeviceAtMega328p, IRQType>;
        
    } // namespace internal   
} // namespace holmes

