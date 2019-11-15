

#include <hal/InterruptVectorTable.hpp>
#include <hal/interrupt_config/ExternalInterrupt.hpp>
#include <config.hpp>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief These are being implemented in _example_impl.cpp
 *      for each target specifically.
 * @param int 
 */
extern void delay_ms(unsigned int);
extern void InitGPIO();
extern void InitExtInt0();
extern void PushButton_Handler();
extern void ToggleLED();

void Blinking(unsigned int ms, unsigned int times = 10)
{
    for (ValueType i = 0; i < times; i++)
    {
        delay_ms(ms);
        ToggleLED();
    } 
}

using IndexType = ExternalInterrupt::IndexType;
using TriggerType = ExternalInterrupt::TriggerType;


int main()
{   
    constexpr auto IDX_INT0 = IndexType::IDX_INT0;
    constexpr auto EDGE_RISING = TriggerType::EDGE_RISING;

    ValueType IRQIndex = IRQ_INDEX;
    InitGPIO();         // Initialize Power, LED and Pushbutton
    //InitExtInt0();      // configure, how the interrupt is triggered(EINT0).

    
    ExternalInterrupt exti0 = {IDX_INT0, EDGE_RISING};
    exti0.apply();

    auto& vectorTable = InterruptVectorTable::getInstance();    // move vector table into singleton/RAM/ aligned memory block
    vectorTable.setCallback(IRQIndex, PushButton_Handler);
    vectorTable.enableISR(IRQIndex);
    vectorTable.disableIRQ();

    // check global IRQs
    if (!vectorTable.isEnabled())
    {
        Blinking(100, 20);
    }
    delay_ms(10000);

    // check specific ISR
    if (vectorTable.isEnabled(IRQIndex))
    {
        Blinking(100, 20);
    }
  
    vectorTable.enableIRQ();

    delay_ms(10000);
    while(1)
    {
        vectorTable.triggerIRQ(IRQIndex);
        delay_ms(1000);
    } 
}


#ifdef __cplusplus
}
#endif