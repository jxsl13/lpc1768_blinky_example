#pragma once

#include "../Interrupt.hpp" // all needed includes, interface, ValueType, etc.


/**
 * @brief Available external interrupts
 */
enum class ExternalInterrupt::IndexType : ValueType {
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
enum class ExternalInterrupt::TriggerType : ValueType {
    LEVEL_HIGH      = 0,
    LEVEL_LOW       = 1,
    EDGE_FALLING    = 2, 
    EDGE_RISING     = 3,
};