#pragma once // include this header only once

#if defined LPC1768
#include <target/LPC17xx/LPC17xx.h>

#elif defined ATMEGA328P

#define BOARD ATMEGA328P_XPLAINED_MINI
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#endif // include platform specific headers.