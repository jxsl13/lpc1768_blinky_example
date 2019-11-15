#pragma once // include this header only once

// print c++ standard version
#define XSTR(x) STR(x)
#define STR(x) #x

#if defined MCB1760

    #include <target/MCB1760/LPC17xx.h>

    using ValueType = uint32_t;
    constexpr ValueType VectorsCount = 256;
    constexpr ValueType ValidInterruptCount = 35;
    constexpr ValueType IRQ_INDEX = 18;                 // EINT0 Index

#elif  defined ARDUINO_UNO \
    || defined MYAVR_BOARD_MK2

    #include <util/delay.h>
    #include <avr/interrupt.h>

    using ValueType = uint8_t;                          // megaAVR® Data Sheet - Page 1 - 8 Bit
    constexpr ValueType VectorsCount = 26;              // Page 66
    constexpr ValueType ValidInterruptCount = 25;       // without reset IRQ index 0

    constexpr ValueType IRQ_INDEX = 1;                  // INT0 Index

    
    #include <target/MYAVR_BOARD_MK2/interrupt_config/ExternalInterrupt.hpp>


#elif defined STM32F407VG

    #include <target/STM32F407VG/stm32f4xx.h>

    using ValueType = uint32_t;
    constexpr ValueType VectorsCount = 256;             // possible, but not necessarily used.
    constexpr ValueType ValidInterruptCount = 82;       
    constexpr ValueType IRQ_INDEX = 6;                  // EXTI0 Index

#endif // include platform specific headers.