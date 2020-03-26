#include <avr/interrupt.h>

#include <utils/BitMacros.hpp>

#include "holmes_platform.hpp"


static auto initGpio() -> void
{
	// make port output & HIGH
    // our power source for external interrupt
    ENABLE(DDRB, DDB4);
    ENABLE(PORTB, PORTB4);

    // make Port B5 output & LOW
    // Connected to an LED - Depending on the state of this, 
    // the LED is either on or off.
    ENABLE(DDRB, DDB5);
    DISABLE(PORTB, PORTB5);

    // Make Port D2 Input External Interrupt
    DISABLE(DDRD, DDD2);
    ENABLE(PORTD, PORTD2);
}

static auto initVectorTable() -> void
{
	(void)holmes::instances::vectorTable();
}

static auto initEXTI() -> void
{
    // Manual - Page 80
    // The falling edge of INT0 generates an interrupt request.
    ENABLE(EICRA, ISC01);
    DISABLE(EICRA, ISC00);
}

auto holmes::init() -> void
{
    initVectorTable();
    initGpio();
    initEXTI();	
}