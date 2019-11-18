#pragma once

#include "../Interrupt.hpp" // all needed includes, interface, ValueType, etc.


//TODO: Implement this
enum class ExternalInterrupt::IndexType : ValueType {
    IDX_EXTI0 = 0,
    IDX_EXTI1 = 1,
    IDX_EXTI2 = 2,
    IDX_EXTI3 = 3,
    IDX_EXTI4 = 4,
    IDX_EXTI5 = 5,
    IDX_EXTI6 = 6,
    IDX_EXTI7 = 7,
    IDX_EXTI8 = 8,
    IDX_EXTI9 = 9,
    IDX_EXTI10 = 10,
    IDX_EXTI11 = 11,
    IDX_EXTI12 = 12,
    IDX_EXTI13 = 13,
    IDX_EXTI14 = 14,
    IDX_EXTI15 = 15,
    IDX_EXTI16 = 16,
    IDX_EXTI17 = 17,
    IDX_EXTI18 = 18,
    IDX_EXTI19 = 19,
    IDX_EXTI20 = 20,
    IDX_EXTI21 = 21,
    IDX_EXTI22 = 22,
};

/**
 * Rising trigger selection register (EXTI_RTSR)
 * Falling trigger selection register (EXTI_FTSR)
 * 
 * EXTI_RTSR    EXTI_FTSR
 *      0           0   Nothing
 *      0           1   falling edge triggers the interrupt
 *      1           0   rising edge triggers the interuupt
 *      1           1   both edges trigger the interrupt
 * 
 */
enum class ExternalInterrupt::TriggerType : ValueType {
    EDGE_FALLING            = 1,
    EDGE_RISING             = 2,
    EDGE_FALLING_RISING     = 3,
};