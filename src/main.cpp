#include "proxy.h"

#ifdef __cplusplus
extern "C" {
#endif


#if defined MCB1760

void delay_ms(unsigned int ms)
{
    volatile unsigned int i,j;
    for(i=0;i<ms;i++)
    for(j=0;j<6000;j++);
}


/* start the main program */
int main(void) 
{
    SystemInit();                    //Clock and PLL configuration 
    LPC_PINCON->PINSEL4 = 0x00000000;//Configure the PORT2 Pins as GPIO;
    LPC_GPIO2->FIODIR = 0xffffffff;  //Configure the PORT2 pins as OUTPUT;

    while(1)
    {
        LPC_GPIO2->FIOPIN = 0x00000000;      // Make all the Port pins as low  
        delay_ms(1000);

        LPC_GPIO2->FIOPIN = 0xffffffff;     // Make all the Port pins as high  
        delay_ms(1000);  
        #pragma warning("lpc main")
    }
}

#elif defined ARDUINO_UNO || defined MYAVR_BOARD_MK2


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

#elif defined STM32F407VG

#pragma warning("STM32")
#define LEDPORT (GPIOD)
#define LED1 (12)
#define ENABLE_GPIO_CLOCK (RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN)
#define GPIOMODER (GPIO_MODER_MODER12_0)


void ms_delay(int ms)
{
   while (ms-- > 0) {
      volatile int x=500;
      while (x-- > 0)
         __asm("nop");
   }
}

int main(void)
{
	ENABLE_GPIO_CLOCK; 		 			// enable the clock to GPIO
	LEDPORT->MODER |= GPIOMODER;		// set pins to be general purpose output
	for (;;) {
		ms_delay(500);
		LEDPORT->ODR ^= (1<<LED1); 		// toggle diodes
	}
	
	return 0;
}

#endif // Different mains


#ifdef __cplusplus
}
#endif