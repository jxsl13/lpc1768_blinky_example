
// Target specific interrupt headers
#if defined ATMEGA328P
    #include <controllers/atmega328p/holmes_platform.hpp>     
#elif defined LPC1768
    #include <controllers/lpc1768/holmes_platform.hpp>
#elif defined STM32F407VG
    #include <controllers/stm32f407vg/holmes_platform.hpp>
#endif

// type aliases
using IRQType =             holmes::IRQType;
using ExtIntType =          holmes::ExtIntType;
using ExtIntConfigType =    ExtIntType::ConfigType;
using ExtIntTriggerType =   ExtIntType::TriggerType;

// microcontroller specific configuration
#if defined ATMEGA328P
    IRQType IRQ_EINT0         = IRQType::IRQn_INT0;

    ExtIntConfigType CFG_EXTI0 = ExtIntConfigType::CFG_INT0_PD2;
    ExtIntTriggerType TRIGGER = ExtIntTriggerType::TRIGGER_EDGE_RISING;
#elif defined LPC1768
    IRQType IRQ_EINT0         = IRQType::IRQn_EINT0;

    ExtIntConfigType CFG_EXTI0 = ExtIntConfigType::CFG_EINT0_P2_10;
    ExtIntTriggerType TRIGGER = ExtIntTriggerType::TRIGGER_EDGE_RISING;
#elif defined STM32F407VG
    IRQType IRQ_EINT0         = IRQType::IRQn_EXTI0;
    
    ExtIntConfigType CFG_EXTI0 = ExtIntConfigType::CFG_EXTI0_PA0;
    ExtIntTriggerType TRIGGER = ExtIntTriggerType::TRIGGER_EDGE_FALLING;
#endif

/**
 * @brief These are being implemented in _example_impl.cpp
 *      for each target specifically.
 * @param int 
 */
extern void delay_ms(unsigned int);
extern void ToggleLED();
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
    
    ExtIntType::clearPendingBitOf(CFG_EXTI0);
}

int main()
{   
    holmes::init(); // init GPIO, init vectortable

    auto exti0Cfg = ExtIntType(CFG_EXTI0, TRIGGER);
    exti0Cfg.apply();

    auto& vectorTable = holmes::instances::vectorTable();
    vectorTable.setISR(IRQ_EINT0, []() {
        Blinking(1, 500);           // Blink with ~500 ms delay
        ExtIntType::clearPendingBitOf(CFG_EXTI0);
    });

    vectorTable.enableISR(IRQ_EINT0);
    vectorTable.enableIRQ(); 

    while(1)
    {
        vectorTable.waitForIRQ();
        Blinking(4, 250);           // Blink 4 times with ~250 ms delay
    } 
}
