
#if defined ATMEGA328P
    #include <controllers/atmega328p/holmes_platform.hpp>     // Target specific interrupt header

    using IRQType = holmes::IRQType;
    constexpr IRQType IRQ_EINT0 = IRQType::INT0_IRQn;


#elif defined LPC1768
    #include <controllers/lpc1768/holmes_platform.hpp>
    using IRQType = holmes::IRQType;
    constexpr IRQType IRQ_EINT0 = IRQType::EINT0_IRQn;

#elif defined STM32F407VG
    #include <controllers/stm32f407vg/holmes_platform.hpp>

    using IRQType = holmes::IRQType;
    constexpr IRQType IRQ_EINT0 = IRQType::EXTI0_IRQn;
#endif



/**
 * @brief These are being implemented in _example_impl.cpp
 *      for each target specifically.
 * @param int 
 */
extern void delay_ms(unsigned int);
extern void ToggleLED();
extern void EnableLED();
extern void DisableLED();

// clears external interrupt pending flag
extern void ClearIRQCondition();

void Blinking(unsigned int times = 1, unsigned int ms = 300)
{
    for (unsigned int i = 0; i < times * 2; i++)
    {
        delay_ms(ms);
        ToggleLED();
    } 
}

void PushButton_Handler()
{
    ClearIRQCondition();
}


#ifdef __cplusplus
extern "C" {
#endif


int main()
{   
    

    holmes::init();

    DisableLED();


    auto& vectorTable = holmes::instances::vectorTable();

    vectorTable.setCallback(IRQ_EINT0, PushButton_Handler); // enable external interrupt service routines
    vectorTable.enableISR(IRQ_EINT0);
    

    // enable global interrupts
    vectorTable.enableIRQ(); 


    while(1)
    {
        vectorTable.waitForIRQ();
        Blinking(1, 500);    // toggle twice with 500ms inbetween                
    } 
}


#ifdef __cplusplus
}
#endif


