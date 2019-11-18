#pragma once

#include "../Interrupt.hpp" // all needed includes, interface, ValueType, etc.


//TODO: Implement this
enum class ExternalInterrupt::IndexType : ValueType {
    IDX_EINT0 = 0,
    IDX_EINT1 = 1,
};

//TODO: Implement this
enum class ExternalInterrupt::TriggerType : ValueType {
    LEVEL_LOW       = 0,
    LOGICAL_CHANGE  = 1,
    EDGE_FALLING    = 2, 
    EDGE_RISING     = 3,
    INVALID_IMPLEMENTATION = 99,
};