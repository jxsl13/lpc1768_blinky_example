#pragma once // include this header only once

#if defined MCB1760
#include <target/MCB1760/LPC17xx.h>
#pragma message("LPC")

#elif  defined ARDUINO_UNO \
    || defined MYAVR_BOARD_MK2

#pragma message("NOT LPC")
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#elif defined STM32F407G
#include <target/STM32F407G/stm32f4xx.h>

#endif // include platform specific headers.