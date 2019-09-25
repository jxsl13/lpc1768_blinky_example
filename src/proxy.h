#pragma once // include this header only once

#if defined LPC1768
#include <LPC17xx/LPC17xx.h>
#elif defined ATMEGA328P
#define BOARD ATMEGA328P_XPLAINED_MINI
#include <ATmega328p/asf.h>
#endif // include platform specific headers.