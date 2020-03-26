#include <stdint.h>
#include <utils/BitMacros.hpp>
#include "stm32f4xx.h"


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

void DisableLED()
{
    uint32_t PD12 = 12;
    DISABLE(GPIOD->ODR, PD12);
}
