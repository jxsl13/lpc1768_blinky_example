#pragma once


/**
 * Platform specific headers
 */
#include <avr/interrupt.h>

/*
 * Platform specific definitions
 */
using ValueType = uint8_t;                          // megaAVR® Data Sheet - Page 1 - 8 Bit
constexpr ValueType VectorsCount = 26;              // Page 66 - Vector 0 is not mutable, the other 25 are mutable

/**
 * For testing purposes, this is the specific interrupt index.
 */
constexpr ValueType IRQ_INDEX = 1;                  // INT0 Index
