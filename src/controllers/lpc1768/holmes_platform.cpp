#include "holmes_platform.hpp"

#include <utils/BitMacros.hpp>


static auto initGpio() -> void
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

static auto initVectorTable() -> void
{
	(void)holmes::instances::vectorTable();
}

static auto initEXTI() -> void
{
    ENABLE(LPC_SC->EXTINT, 0);      // Clear Pending interrupts
    ENABLE(LPC_SC->EXTMODE, 0);     // Configure EINTx as Edge Triggered
    ENABLE(LPC_SC->EXTPOLAR, 0);    // Configure EINTx as Falling Edge
}

auto holmes::init() -> void
{
    initGpio();
    initVectorTable();
    initEXTI();
	
}