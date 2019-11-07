#pragma once // include this header only once

#if defined MCB1760
#include <target/MCB1760/LPC17xx.h>
#include <target/MCB1760/Pins.h>

using ValueType = uint32_t;
constexpr ValueType VectorsCount = 256;

#elif  defined ARDUINO_UNO \
    || defined MYAVR_BOARD_MK2
#include <util/delay.h>
#include <avr/interrupt.h>

using ValueType = uint8_t;              // megaAVR® Data Sheet - Page 1 - 8 Bit
constexpr ValueType VectorsCount = 26;  // Page 66

#elif defined STM32F407VG
#include <target/STM32F407VG/stm32f4xx.h>

using ValueType = uint32_t;
constexpr ValueType VectorsCount = 256;

#endif // include platform specific headers.