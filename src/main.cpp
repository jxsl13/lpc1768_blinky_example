
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
    using UIntType = holmes::internal::DeviceAtMega328p::ValueTypeUnsigned;
    UIntType NumInterrupts         = 25;
    IRQType IRQ_EINT0         = IRQType::IRQn_INT0;
    
    ExtIntConfigType CFG_EXTI0 = ExtIntConfigType::CFG_INT0_PD2;
    ExtIntTriggerType TRIGGER = ExtIntTriggerType::TRIGGER_EDGE_RISING;
#elif defined LPC1768
    using UIntType = holmes::internal::DeviceLPC1768::ValueTypeUnsigned;
    UIntType NumInterrupts         = 35;
    IRQType IRQ_EINT0         = IRQType::IRQn_EINT0;

    ExtIntConfigType CFG_EXTI0 = ExtIntConfigType::CFG_EINT0_P2_10;
    ExtIntTriggerType TRIGGER = ExtIntTriggerType::TRIGGER_EDGE_RISING;
#elif defined STM32F407VG
    using UIntType = holmes::internal::DeviceSTM32F407VG::ValueTypeUnsigned;
    UIntType NumInterrupts         = 82;
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

void Example() {
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

// Blinks twice shortly, if correct.
void TestIsEnabledIRQ() {
    auto& vectorTable = holmes::instances::vectorTable();
    vectorTable.enableIRQ();

    if (vectorTable.isEnabledIRQ())
    {
        Blinking(1, 1000);
    } else {
        Blinking(4, 1000);
    } 

    vectorTable.disableIRQ();
    if (!vectorTable.isEnabledIRQ())
    {
        Blinking(1, 1000);
    } else {
        Blinking(4, 1000);
    }

    while(1){} 
}


void TestTriggerISR(IRQType irq) {
    auto& vectorTable = holmes::instances::vectorTable();
    
    vectorTable.setISR(irq, [](){
        NumInterrupts--; // if used for testing
        Blinking(1, 1000);
    });
    
    vectorTable.enableISR(irq);
    vectorTable.enableIRQ();
    
    vectorTable.triggerISR(irq);

}

void TestWaitForIRQ(IRQType irq) {
    
    {
        auto exti0Cfg = ExtIntType(CFG_EXTI0, TRIGGER);
        exti0Cfg.apply();
    }

    auto& vectorTable = holmes::instances::vectorTable();
    vectorTable.setISR(irq, [](){
        // do nothing
    });
    vectorTable.enableISR(irq);
    vectorTable.enableIRQ();

    vectorTable.waitForIRQ();
    Blinking(1, 1000);
}



int main()
{   
    holmes::init(); // init GPIO, init vectortable
    delay_ms(1000);
    
    //TestIsEnabledIRQ();
    // int size = NumInterrupts;
    // for (int i = 1; i < size; i++) {
    //     TestTriggerISR(static_cast<IRQType>(i));
    // }

    // delay_ms(1000);
    // if (NumInterrupts == 0)
    // {
    //     Blinking(1, 1000);
    // } else {
    //     Blinking(NumInterrupts, 5000);
    // }

    TestTriggerISR(static_cast<IRQType>(5));
    
    
    while(1){} 
}
