#include "proxy.h"
#include "hal/InterruptHandler.h"

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


volatile int g_LEDState = 0;

void ToggleLED()
{
    
    switch(g_LEDState)
    {
        case 0:
        {
            // enable LED P1.28
            LPC_GPIO1->FIOSET |= (1 << 28);
            g_LEDState = 1;
            break;
        }
        case 1:
        {
            // disable LED P1.28
            LPC_GPIO1->FIOCLR |= (1 << 28);
            g_LEDState = 0;
            break;
        }
    }
}

void PushButton_Handler()
{
    ToggleLED();
    
    //LPC_GPIOINT->IO2IntClr |= (1 << 10);    // Clear interrupt condition when it has been fired
}

void InitPower()
{
    LPC_SC->PCONP |= (1 << 15);             // Enable power
}

void InitLED()
{
    LPC_PINCON->PINSEL3 &= ~(0b00 << 25);   // LED connected to p1.28 is in GPIO mode (see Table 83)
    LPC_GPIO1->FIODIR |= (1 << 28);         // LED connected to p1.28 is an output pin
}

void InitPushButton()
{
    LPC_PINCON->PINSEL4 &= ~(0b01 << 21);      // Push-button connected to P2.10 is in GPIO mode
    LPC_GPIO2->FIODIR   &= ~(1 << 10);      // P2.10 is an input pin

    LPC_GPIOINT->IO2IntEnF |=  (1 << 10);   // P2.10 reads the falling edges to generate an interrupt

    NVIC_SetVector(EINT0_IRQn, (uint32_t)PushButton_Handler);
    NVIC_EnableIRQ(EINT0_IRQn);             // IRQ is enabled in NVIC.

}


/* start the main program */
int main(void) 
{
    SystemInit();                    //Clock and PLL configuration 
    InterruptHandler& IRQHandler = InterruptHandler::GetInstance();
    (void)IRQHandler;

    InitPower();
    InitLED();
    InitPushButton();

    //

    
    

    while(1)
    {
        delay_ms(1000);
        //ToggleLED();
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

#define LEDPORT (GPIOD)
#define LED1 (12)
#define ENABLE_GPIO_CLOCK (RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN)
#define GPIOMODER (GPIO_MODER_MODER12_0)


void ms_delay(int ms)
{
   while (ms-- > 0) {
      volatile int x=1000;
      while (x-- > 0)
         __asm("nop");
   }
}

int main(void)
{
	ENABLE_GPIO_CLOCK; 		 			// enable the clock to GPIO
	LEDPORT->MODER |= GPIOMODER;		// set pins to be general purpose output
	for (;;) {
		ms_delay(1000);
		LEDPORT->ODR ^= (1<<LED1); 		// toggle diodes
	}
	
	return 0;
}

#endif // Different mains


#ifdef __cplusplus
}
#endif