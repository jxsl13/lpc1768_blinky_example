#pragma once

/**
 * Target specific includes
 */
#include "LPC17xx.h"

/**
 * @brief Target specific value type
 */
using ValueType = uint32_t;
    
/**
 * @brief Alignment of the Interrupt Vector Table
 */
constexpr ValueType Alignment = 256;

/**
 * @brief Number of available, nor necessarily accessible Interrupt vectors

 */
constexpr ValueType VectorsCount = 35 + 16;

/**
 * For testing purposes, this defines one specific interrupt.
 */
constexpr ValueType IRQ_INDEX = 18;                 // EINT0 Index