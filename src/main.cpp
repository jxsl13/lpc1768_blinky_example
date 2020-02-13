#include <util/delay.h>
#include <avr/interrupt.h>
#include <utils/BitMacros.hpp>

void InitGPIO()
{
    // Power source
    ENABLE(DDRB, DDB4);     // set PB4 as output (Manual Page 86)
    ENABLE(PORTB, PORTB4);  // set PB4 as HIGH

    // LED
    ENABLE(DDRB, DDB5);     // set PB5 as output
    DISABLE(PORTB, PORTB5); // set PB5 as LOW

    // Make Port D2 Input External Interrupt
    DISABLE(DDRD, DDD2);    // set PD2 as input
    ENABLE(PORTD, PORTD2);  // trie-state
}

void InitExtInt0()
{
    ENABLE(EICRA, ISC01);   // INT0 is triggered by a ...
    DISABLE(EICRA, ISC00);  // ... falling edge (Manual Page 80)

    ENABLE(EIMSK, INT0);    // enable INT0 (Manual Page 81)
    sei();                  // enable global Interrupts
}

void ToggleLED()
{
    TOGGLE(PORTB, PORTB5);    // toggle between HIGH & LOW
}

void delay_ms(unsigned int ms)
{
    while (ms-- > 0)
    {  
        _delay_ms(1);           // limited delay
    }
}

ISR(INT0_vect)
{
    for (int i = 0; i < 2; i++) 
    {
        ToggleLED();
        delay_ms(300);
    }
    ENABLE(EIFR, 0);    // clear pending flag (Manual Page 81)
}

int main()
{
    InitGPIO();
    InitExtInt0();
    while(true){/* do nothing */};
}

