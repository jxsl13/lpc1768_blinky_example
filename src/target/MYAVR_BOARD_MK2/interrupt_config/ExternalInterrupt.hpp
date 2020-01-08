#pragma once

#include "../Interrupt.hpp" // all needed includes, interface, ValueType, etc.

enum class ExternalInterrupt::IndexType : ValueType {
    EINT0_PD2 = 0,
    EINT1_PD3 = 1,
};


enum class ExternalInterrupt::TriggerType : ValueType {
    LEVEL_LOW               = 0,
    EDGE_FALLING_RISING     = 1,
    EDGE_FALLING            = 2, 
    EDGE_RISING             = 3,
};