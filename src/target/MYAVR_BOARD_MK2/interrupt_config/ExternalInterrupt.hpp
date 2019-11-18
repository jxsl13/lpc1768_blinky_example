#pragma once

#include "../Interrupt.hpp" // all needed includes, interface, ValueType, etc.

enum class ExternalInterrupt::IndexType : ValueType {
    IDX_INT0 = 0,
    IDX_INT1 = 1,
};


enum class ExternalInterrupt::TriggerType : ValueType {
    LEVEL_LOW       = 0,
    LOGICAL_CHANGE  = 1,
    EDGE_FALLING    = 2, 
    EDGE_RISING     = 3,
};