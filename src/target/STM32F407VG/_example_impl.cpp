#include "Interrupt.hpp"
#include <utils/BitMacros.hpp>

#ifdef __cplusplus
extern "C" {
#endif
enum PortX : ValueType
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

enum Trigger : ValueType
{
    Trigger_None = 0b00,
    Trigger_Rising_Edge = 0b01,             
    Trigger_Falling_Edge = 0b10,           
    Trigger_Logical_Change = Trigger_Rising_Edge | Trigger_Falling_Edge,
};

void delay_ms(unsigned int ms)
{
   while (ms-- > 0) {
      volatile int x=6000;
      while (x-- > 0)
         __asm("nop");
   }
}

void InitGPIO()
{

    // LED
    ValueType PD12 = 12;
    ENABLE(RCC->AHB1ENR, PD);               // enable clock on Port D
    ENABLE(GPIOD->MODER, (2 * PD12));       // 0b01 -> General Purpose ouput
    DISABLE(GPIOD->MODER, (2 * PD12) + 1);  // set PD12 to Output

    // PA0 - External Input
    ValueType PA0 = 0;
    ENABLE(RCC->AHB1ENR, PA);           // Enable Clock for Port A
    DISABLE(GPIOA->MODER, 2 * PA0);     // set PA0 to be an input
    DISABLE(GPIOA->MODER, 2 * PA0 + 1); // PA0 input   


    // PB3 - Input
    ValueType PB3 = 3;
    ENABLE(RCC->AHB1ENR, PB);           // Enable Clock for Port A
    DISABLE(GPIOB->MODER, 2 * PB3);     // set PA0 to be an input
    DISABLE(GPIOB->MODER, 2 * PB3 + 1); // PA0 input  
}


void InitExtInt0()
{
    // Trigger EXTI0 with PA0 (EXTI[X] -> P[Y][X])
    ValueType Port = PA;
    ValueType EXTIX = 0;
    ValueType Trigger = Trigger_Rising_Edge;

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

void ClearIRQCondition()
{
    ValueType IRQIndex = EXTI0_IRQn;

    if (IRQIndex > 22)
    {
        return; // mask prevents any other bits from being set.
    }

    //EXTI->PR |= ((1 << IRQIndex) & 0x7FFFFF); // first [22:0]
    ENABLE(EXTI->PR, IRQIndex);    
}


void ToggleLED()
{
    ValueType PD12 = 12;
    TOGGLE(GPIOD->ODR, PD12);
}

void PushButton_Handler()
{
    ToggleLED();
    // clear interrupt condition (EXTI Pending Bit Register)
    // otherwise you will get tailchained interrupts.
    ClearIRQCondition();
}


#ifdef __cplusplus
}
#endif

