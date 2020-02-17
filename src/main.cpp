#if defined ATMEGA328P

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
    ENABLE(PORTD, PORTD2);  // tri-state
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

ISR(INT0_vect, ISR_NOBLOCK)
{
    for (int i = 0; i < 2; i++) 
    {
        ToggleLED();
        delay_ms(300);
    }
    //ENABLE(EIFR, INTF0);    // clear pending flag (Manual Page 81)
}

int main()
{
    InitGPIO();
    InitExtInt0();
    while(true){/* do nothing */};
}

#elif defined LPC1768
#include "controllers/lpc1768/LPC17xx.h"
#include <utils/BitMacros.hpp>

void InitGPIO()
{
    SystemInit();                    // startup board in a predefined state

    // LED - P1.28 - Pin Function
    DISABLE(LPC_PINCON->PINSEL3, 24);// P1.28 AS GPIO Pin
    DISABLE(LPC_PINCON->PINSEL3, 25);// ...(Manual Pages 114, 119, 120)

    // LED - P1.28 - GPIO Configuration
    ENABLE(LPC_GPIO1->FIODIR, 28); // P1.28 As output(Manual Page 133)
    DISABLE(LPC_GPIO1->FIOPIN, 28);// P1.28 Set as LOW(Manual Pages 132,150)

    // Pushbutton - P2.10 (INT0)
    DISABLE(LPC_PINCON->PINSEL4, 21);// P2.10 As external Interrupt(EINT0) 
    ENABLE(LPC_PINCON->PINSEL4, 20); // ...(Manual Page 120)
}

void InitExtInt0()
{
    __disable_irq();             // cmsis_gcc.h - Disable interrupts globally

    ENABLE(LPC_SC->EXTINT, 0);   // EINT0 Clear pending bit(Manual Pages 26,27)
    ENABLE(LPC_SC->EXTMODE, 0);  // EINT0 Edge sensitive(Manual Page 26, 28)
    DISABLE(LPC_SC->EXTPOLAR, 0);// EINT0 Falling edge(Manual Page 26,28)

    NVIC_EnableIRQ(EINT0_IRQn);// LPC17xx.h - Enable external Interrupt(EINT0)
    __enable_irq();            // cmsis_gcc.h - Enable interrupts globally
}

void delay_ms(unsigned int ms)
{
    // Not necessarily accurate, but enough of a delay for testing purposes
    // SystemFrequency is defined in system_LPC17xx.h
    unsigned int resetValue = 8 * (SystemFrequency / (SystemFrequency / 1000));
    while (ms-- > 0)
    {
        unsigned int x = resetValue;
        while (x-- > 0)
            __asm("nop");
    }
}

void ToggleLED()
{
    TOGGLE(LPC_GPIO1->FIOPIN, 28);// P2.10 Toggle state (Manual Page 133)
}

extern "C" void EINT0_IRQHandler()// Definitions in startup_LPC17xx.S
{
    ToggleLED();
    ENABLE(LPC_SC->EXTINT, 0);    // clear pending flag
}

int main()
{
    InitGPIO();     // configure GPIO
    InitExtInt0();  // configure external interrupt
    while(true){};
}
#elif defined STM32F407VG
#include "controllers/stm32f407vg/stm32f4xx.h"

#endif
