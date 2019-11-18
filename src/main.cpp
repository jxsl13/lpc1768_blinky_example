

#if defined MCB1760
    #include <target/MCB1760/Interrupt.hpp>
#elif  defined ARDUINO_UNO || defined MYAVR_BOARD_MK2
    #include <target/MYAVR_BOARD_MK2/Interrupt.hpp>
#elif defined STM32F407VG
    #include <target/STM32F407VG/Interrupt.hpp>
#endif // include platform specific headers.


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

void Blinking(unsigned int times = 5, unsigned int ms = 300)
{
    for (ValueType i = 0; i < times; i++)
    {
        delay_ms(ms * 2);
        ToggleLED();
    } 
}

using IndexType = ExternalInterrupt::IndexType;
using TriggerType = ExternalInterrupt::TriggerType;


int main()
{   
    constexpr auto IDX_INT0 = static_cast<IndexType>(IRQ_INDEX);
    constexpr auto EDGE_RISING = TriggerType::EDGE_RISING;

    IRQType IRQIndex = static_cast<IRQType>(IRQ_INDEX);
    InitGPIO();         // Initialize Power, LED and Pushbutton
    //InitExtInt0();      // configure, how the interrupt is triggered(EINT0).

    
    ExternalInterrupt exti0 = {IDX_INT0, EDGE_RISING};
    exti0.apply();


    auto& vectorTable = InterruptVectorTable::getInstance();    // move vector table into singleton/RAM/ aligned memory block
    vectorTable.setCallback(IRQIndex, PushButton_Handler);
    vectorTable.enableISR(IRQIndex);

    vectorTable.enableIRQ();

    delay_ms(5000);
    Blinking(static_cast<unsigned int>(exti0.getTrigger()));
    

    delay_ms(10000);
    exti0.retrieve();
    unsigned int result = static_cast<unsigned int>(exti0.getTrigger());
    Blinking(result);

    if (!result)
    {
        ToggleLED();
    }
    

    while(1)
    {
        
    } 
}


#ifdef __cplusplus
}
#endif