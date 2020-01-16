#include "Interrupt.hpp"
#include <utils/BitMacros.hpp>



void delay_ms(unsigned int ms)
{
    volatile unsigned int resetValue = 8 * (SystemFrequency / (SystemFrequency / 1000));
    while (ms-- > 0)
    {
        volatile unsigned int x = resetValue;

        while (x-- > 0)
            __asm("nop");
    }
}


void InitGPIO()
{   
    SystemInit();                       // startup board in a predefined state

    /* Enable Power */
    ENABLE(LPC_SC->PCONP, 15);          // Enable power

    /* Init LED */
    DISABLE(LPC_PINCON->PINSEL3, 24);   // LED connected to P1.28 is in GPIO mode (see Table 83)
    DISABLE(LPC_PINCON->PINSEL3, 25);   // second bit of P1.28

    ENABLE(LPC_GPIO1->FIODIR, 28);      // Configure LED pins as OUTPUT - P1.28
    ENABLE(LPC_GPIO1->FIOCLR, 28);      // set P1.28 to LOW
    ENABLE(LPC_GPIO1->FIOPIN, 28);

    /* Init Pushbutton */
    ENABLE(LPC_PINCON->PINSEL4, 20);    // Configure P2_10, as EINT0 (see data sheet of PINSEL4)
    DISABLE(LPC_GPIO2->FIODIR, 10);     // P2.10 is an input pin
    ENABLE(LPC_GPIOINT->IO2IntEnF, 10); // P2.10 reads the falling edges to generate an interrupt
}

void InitExtInt0()
{
    ENABLE(LPC_SC->EXTINT, 0);      // Clear Pending interrupts
    ENABLE(LPC_SC->EXTMODE, 0);     // Configure EINTx as Edge Triggered
    ENABLE(LPC_SC->EXTPOLAR, 0);    // Configure EINTx as Falling Edge      
}

void InitWatchDogTimer()
{
    // use PLL0's source
    uint32_t clockSource = LPC_SC->CLKSRCSEL & 0b11;
    LPC_WDT->WDCLKSEL = clockSource;    //PCLK. Selects the APB peripheral clock (watchdog pclk) as the Watchdog clock source.

    LPC_WDT->TC = 5000000;            // timout interval
    LPC_WDT->MOD = 0x03;              // enable interrupt trigger, hardware reset & enable WDT, not recoverable

    // WDT reset sequence to prevent timeout
    LPC_WDT->FEED = 0xAA;
    LPC_WDT->FEED = 0x55;
}

void FeedWatchDog()
{
    LPC_WDT->FEED = 0xAA;
    LPC_WDT->FEED = 0x55;
}

void ToggleLED()
{
    TOGGLE(LPC_GPIO1->FIOPIN, 28);
}
void EnableLED()
{
    ENABLE(LPC_GPIO1->FIOPIN, 28);
}

void DisableLED()
{
    DISABLE(LPC_GPIO1->FIOPIN, 28);
}

void ClearIRQCondition()
{
    ENABLE(LPC_SC->EXTINT, 0); // Clear Interrupt Flag/Pending Bit -> 0 = EINT0
}


