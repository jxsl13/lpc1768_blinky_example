#include <stdint.h>
#include <utils/BitMacros.hpp>
#include "Interrupt.hpp"


void delay_ms(unsigned int ms)
{
    volatile uint32_t resetValue = 6 * SystemCoreClock / (SystemCoreClock / 1000);
    while (ms-- > 0)
    {
        volatile int x = resetValue;
        while (x-- > 0)
            __asm("nop");
    }
}

void InitWatchDogTimer()
{
    // reference: https://www.st.com/content/ccc/resource/training/technical/product_training/7e/3d/fb/30/e7/ec/45/b6/STM32L4_WDG_TIMERS_WWDG.pdf/files/STM32L4_WDG_TIMERS_WWDG.pdf/jcr:content/translations/en.STM32L4_WDG_TIMERS_WWDG.pdf

    // reference: https://github.com/STMicroelectronics/STM32CubeF4/blob/master/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_wwdg.c
    // reference: https://github.com/STMicroelectronics/STM32CubeF4/blob/master/Drivers/STM32F4xx_HAL_Driver/Inc/stm32f4xx_ll_wwdg.h

    constexpr uint32_t WWDGEN = 11; // manual page 185/ RCC_APB1ENR_WWDGEN
    ENABLE(RCC->APB1ENR, WWDGEN);   // enable clock

    WWDG->CFR |= WWDG_CFR_WDGTB1 | WWDG_CFR_WDGTB0; // set prescaler to 8 (Pages 715/718)
    WWDG->CFR |= 0x3f & 63;                         // contain the window value to be compared to the downcounter.
    WWDG->CFR |= WWDG_CFR_EWI;                      // enable interrupt created by the wd, this interrupt can ONLY BE CLEARED by a hardware reset(Page 718)

    WWDG->SR &= ~WWDG_SR_EWIF; // set to 0, clears interrupt flag

    WWDG->CR |= WWDG_CR_T6;    // set bit 6 to prevent immediate reset of the system
    WWDG->CR |= 0x3f & 63;     // contain the value of the watchdog counter
    WWDG->CR |= WWDG_CR_WDGA;  // Enable watchdog
    WWDG->CFR |= WWDG_CFR_EWI; // enable early wakeup interrupt
}

void FeedWatchDog()
{
    WWDG->CR |= 0x7f & 127; // reset watchdog counter value
    //WWDG->CFR |= WWDG_CFR_EWI; // clear pending flag
}

void ClearIRQCondition()
{
    constexpr uint32_t IRQIndex = EXTI0_IRQn;

    if (IRQIndex > 22)
    {
        // first [22:0] allowed
        return;
    }
    ENABLE(EXTI->PR, IRQIndex);
}

void ToggleLED()
{
    uint32_t PD12 = 12;
    TOGGLE(GPIOD->ODR, PD12);
}

void EnableLED()
{
    uint32_t PD12 = 12;
    ENABLE(GPIOD->ODR, PD12);
}

void DisableLED()
{
    uint32_t PD12 = 12;
    DISABLE(GPIOD->ODR, PD12);
}
