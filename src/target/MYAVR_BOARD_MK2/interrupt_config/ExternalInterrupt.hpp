#pragma once
#include <hal/interrupt_config/ExternalInterrupt.hpp>

enum class ExternalInterrupt::InterruptIndexType : ValueType {
    IDX_INT0 = 0,
    IDX_INT1 = 1,
};


enum class ExternalInterrupt::TriggerType : ValueType {
    LEVEL_LOW       = 0,
    LOGICAL_CHANGE  = 1,
    EDGE_FALLING    = 3, 
    EDGE_RISING     = 4,
};