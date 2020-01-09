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
    volatile ValueType resetValue = 6 * SystemCoreClock / (SystemCoreClock / 1000);
    while (ms-- > 0) {
        volatile int x = resetValue;
        while (x-- > 0)
            __asm("nop");
   }
}

void InitGPIO()
{

    // LED
    constexpr ValueType PD12 = 12;
    ENABLE(RCC->AHB1ENR, PD);               // enable clock on Port D
    ENABLE(GPIOD->MODER, 2 * PD12);       // 0b01 -> General Purpose ouput
    DISABLE(GPIOD->MODER, (2 * PD12) + 1);  // set PD12 to Output

    // PA0 - External Input
    constexpr ValueType PA0 = 0;
    ENABLE(RCC->AHB1ENR, PA);           // Enable Clock for Port A
    DISABLE(GPIOA->MODER, 2 * PA0);     // set PA0 to be an input
    DISABLE(GPIOA->MODER, 2 * PA0 + 1); // PA0 input   


    // PB3 - Input
    constexpr ValueType PB3 = 3;
    ENABLE(RCC->AHB1ENR, PB);           // Enable Clock for Port A
    DISABLE(GPIOB->MODER, 2 * PB3);     // set PA0 to be an input
    DISABLE(GPIOB->MODER, 2 * PB3 + 1); // PA0 input  
}


void InitExtInt0()
{
    // Trigger EXTI0 with PA0 (EXTI[X] -> P[Y][X])
    constexpr ValueType Port = PA;
    constexpr ValueType EXTIX = 0;
    constexpr ValueType Trigger = Trigger_Rising_Edge;

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

static ValueType GetBitPosition(ValueType value)
{
    ValueType result = 0;

    while(value > 0)
    {
        value >>= 1;
        result++;
    }

    return result;
}

void InitWatchDogTimer()
{
    // reference: https://www.st.com/content/ccc/resource/training/technical/product_training/7e/3d/fb/30/e7/ec/45/b6/STM32L4_WDG_TIMERS_WWDG.pdf/files/STM32L4_WDG_TIMERS_WWDG.pdf/jcr:content/translations/en.STM32L4_WDG_TIMERS_WWDG.pdf

    // reference: https://github.com/STMicroelectronics/STM32CubeF4/blob/master/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_wwdg.c
    // reference: https://github.com/STMicroelectronics/STM32CubeF4/blob/master/Drivers/STM32F4xx_HAL_Driver/Inc/stm32f4xx_ll_wwdg.h
    
    constexpr ValueType WWDGEN = 11;    // manual page 185/ RCC_APB1ENR_WWDGEN
    ENABLE(RCC->APB1ENR, WWDGEN);   // enable clock
    

    WWDG->CFR |= WWDG_CFR_WDGTB1 | WWDG_CFR_WDGTB0; // set prescaler to 8 (Pages 715/718)
    WWDG->CFR |= 0x3f & 63;    // contain the window value to be compared to the downcounter.
    WWDG->CFR |= WWDG_CFR_EWI; // enable interrupt created by the wd, this interrupt can ONLY BE CLEARED by a hardware reset(Page 718)

    WWDG->SR &= ~WWDG_SR_EWIF; // set to 0, clears interrupt flag

    WWDG->CR |= WWDG_CR_T6; // set bit 6 to prevent immediate reset of the system
    WWDG->CR |= 0x3f & 63; // contain the value of the watchdog counter
    WWDG->CR |= WWDG_CR_WDGA; // Enable watchdog
    WWDG->CFR |= WWDG_CFR_EWI; // enable early wakeup interrupt

}

void FeedWatchDog()
{
    WWDG->CR |= 0x7f & 127; // reset watchdog counter value
    //WWDG->CFR |= WWDG_CFR_EWI; // clear pending flag
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

void EnableLED()
{
    ValueType PD12 = 12;
    ENABLE(GPIOD->ODR, PD12);
}

void DisableLED()
{
    ValueType PD12 = 12;
    DISABLE(GPIOD->ODR, PD12);
}

static void PushButton_Handler()
{
    ToggleLED();
    // clear interrupt condition (EXTI Pending Bit Register)
    // otherwise you will get tailchained interrupts.
    ClearIRQCondition();
}


#ifdef __cplusplus
}
#endif

