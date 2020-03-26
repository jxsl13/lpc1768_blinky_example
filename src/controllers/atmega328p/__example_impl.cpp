#include <avr/io.h>
#include <util/delay.h>         // avr library
#include "Interrupt.hpp"        // target specific headers
#include <utils/BitMacros.hpp>  // hal tools


void delay_ms(unsigned int ms)
{
    while (ms-- > 0)
    {  
        _delay_ms(1);
    }
}

void InitGPIO()
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

void InitExtInt0()
{
    // Manual - Page 80
    // The falling edge of INT0 generates an interrupt request.
    ENABLE(EICRA, ISC01);
    DISABLE(EICRA, ISC00);
}

void ToggleLED()
{
    // toggle the LED
    TOGGLE(PORTB, PORTB5);    
}

void DisableLED()
{
    DISABLE(PORTB, PORTB5); 
}

void ClearIRQCondition()
{
    ENABLE(EIFR, 0); // clear more or less pending bit.
}




