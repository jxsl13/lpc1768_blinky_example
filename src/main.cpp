#include "proxy.h"

#ifdef __cplusplus
extern "C" {
#endif


#if defined LPC1768

void delay_ms(unsigned int ms)
{
volatile unsigned int i,j;
for(i=0;i<ms;i++)
for(j=0;j<10000;j++);
}

/* start the main program */
int main() 
{
    SystemInit();                    //Clock and PLL configuration 
    LPC_PINCON->PINSEL4 = 0x00000000;//Configure the PORT2 Pins as GPIO;
    LPC_GPIO2->FIODIR = 0xffffffff;  //Configure the PORT2 pins as OUTPUT;

    LPC_GPIO2->FIOPIN = 0x00000000;      // Make all the Port pins as low  
    delay_ms(1000);

    //LPC_GPIO2->FIOPIN = 0xffffffff;     // Make all the Port pins as high  
    //delay_ms(1000);

    while(1)
    {
        //LPC_GPIO2->FIOPIN = 0x00000000;      // Make all the Port pins as low  
        //delay_ms(1000);

        //LPC_GPIO2->FIOPIN = 0xffffffff;     // Make all the Port pins as high  
        //delay_ms(1000);
    }
}
#elif defined ATMEGA328P


int main(void)
{
    // make the LED pin an output for PORTB5
    DDRB = 1 << DDB5;       // DDB5 = 5

    while (1)
    {
        _delay_ms(200);

        // toggle the LED
        PORTB ^= 1 << PB5;  // PB5 = 5
    }

    return 0;
}

#endif // Different Mains


#ifdef __cplusplus
}
#endif