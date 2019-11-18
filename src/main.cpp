

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
    for (ValueType i = 0; i < times * 2; i++)
    {
        delay_ms(ms);
        ToggleLED();
    } 
}

using IndexType = ExternalInterrupt::IndexType;
using TriggerType = ExternalInterrupt::TriggerType;

// compile time initialization, less to write.
constexpr IndexType init(const int param)
{
    return static_cast<IndexType>(param);
}


int main()
{   
    constexpr IndexType EINT0 = init(0);
    constexpr IndexType EINT1 = init(1);
    constexpr TriggerType EDGE_FALLING = TriggerType::EDGE_FALLING;
    constexpr TriggerType EDGE_RISING = TriggerType::EDGE_RISING;

    IRQType IRQIndex = static_cast<IRQType>(IRQ_INDEX);
    InitGPIO();         // Initialize Power, LED and Pushbutton
    //InitExtInt0();      // configure, how the interrupt is triggered(EINT0).

    
    ExternalInterrupt exti0 = {EINT0, EDGE_FALLING};
    ExternalInterrupt exti1 = {EINT1, EDGE_RISING};
    
    exti0.apply();
    exti1.apply();


    auto& vectorTable = InterruptVectorTable::getInstance();    // move vector table into singleton/RAM/ aligned memory block
    vectorTable.setCallback(IRQIndex, PushButton_Handler);
    vectorTable.enableISR(IRQIndex);

    vectorTable.enableIRQ();

    exti0.retrieveFrom(EINT1);
    Blinking((unsigned int)exti0.getTrigger());

    while(1)
    {
        // delay_ms(500);
        // vectorTable.triggerIRQ(IRQIndex);
    } 
}


#ifdef __cplusplus
}
#endif