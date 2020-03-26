
#include "stm32f4xx.h"

#include <utils/BitMacros.hpp>

#include "holmes_platform.hpp"


enum PortX : uint32_t
{
        PA = 0,
        PB = 1,
        PC = 2,
        PD = 3,
        PE = 4,
        PF = 5,
        PG = 6,
        PH = 7,
        PI = 8,
};

enum Trigger : uint32_t
{
    Trigger_None = 0b00,
    Trigger_Rising_Edge = 0b01,             
    Trigger_Falling_Edge = 0b10,           
    Trigger_Logical_Change = Trigger_Rising_Edge | Trigger_Falling_Edge,
};


static auto initGpio() -> void
{
	// LED
    constexpr uint32_t PD12 = 12;
    ENABLE(RCC->AHB1ENR, PD);               // enable clock on Port D
    ENABLE(GPIOD->MODER, 2 * PD12);         // 0b01 -> General Purpose ouput
    DISABLE(GPIOD->MODER, (2 * PD12) + 1);  // set PD12 to Output

    // PA0 - External Input
    constexpr uint32_t PA0 = 0;
    ENABLE(RCC->AHB1ENR, PA);           // Enable Clock for Port A
    DISABLE(GPIOA->MODER, 2 * PA0);     // set PA0 to be an input
    DISABLE(GPIOA->MODER, 2 * PA0 + 1); // PA0 input   


    // PB3 - Input
    constexpr uint32_t PB3 = 3;
    ENABLE(RCC->AHB1ENR, PB);           // Enable Clock for Port B
    DISABLE(GPIOB->MODER, 2 * PB3);     // set PB3 to be an input
    DISABLE(GPIOB->MODER, 2 * PB3 + 1); // PB3 input  
}

static auto initVectorTable() -> void
{
	(void)holmes::instances::vectorTable();
}

static auto initEXTI() -> void
{
    // Trigger EXTI0 with PA0 (EXTI[X] -> P[Y][X])
    constexpr uint32_t Port = PA;
    constexpr uint32_t EXTIX = 0;
    constexpr uint32_t Trigger = Trigger_Rising_Edge;

    /* 
        P[X] will trigger the interrupt
        let Port be A and EXTIX be 1,
        this will lead to PA1 to trigger the interupt EXTI1
    */
    SYSCFG->EXTICR[EXTIX / 4] |= (Port << (EXTIX % 4));


    if (!Trigger)
        return;
    
    if (EXTIX > 22)
        return;
    
    /* Clear EXTI line configuration */
    /**
     * LINE := (1 << Line_Number)
     * 
     * We trigger events and interrupts at the same time!
     */
    ENABLE(EXTI->IMR, EXTIX);  // InterruptMaskRegister -> EXTI_IMR_MR0 = 0
    ENABLE(EXTI->EMR, EXTIX);  // EventMaskRegister ->     EXTI_EMR_MR0 = 0

    if (Trigger == Trigger_Rising_Edge)
    {
        ENABLE(EXTI->RTSR, EXTIX);
        DISABLE(EXTI->FTSR, EXTIX);
    }
    else if(Trigger == Trigger_Falling_Edge)
    {
        DISABLE(EXTI->RTSR, EXTIX);
        ENABLE(EXTI->FTSR, EXTIX);
    }
    else
    {
        ENABLE(EXTI->RTSR, EXTIX);
        ENABLE(EXTI->FTSR, EXTIX);
    }
}

auto holmes::init() -> void
{
    initVectorTable();
	initGpio();
    initEXTI();
}