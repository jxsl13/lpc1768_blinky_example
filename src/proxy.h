#pragma once // include this header only once

#if defined LPC1768
#include <target/LPC1768/LPC17xx.h>

#elif defined ATMEGA328P

#define BOARD ATMEGA328P_XPLAINED_MINI
#include <target/ATMEGA328P/asf.h>

#endif // include platform specific headers.