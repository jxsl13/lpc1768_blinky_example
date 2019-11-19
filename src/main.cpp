
#if defined MCB1760

    #include <target/MCB1760/Interrupt.hpp>             // Target specific interrupt header

    constexpr ExternalInterrupt::IndexType EINT0 = ExternalInterrupt::IndexType::EINT0;
    constexpr ExternalInterrupt::IndexType EINT1 = ExternalInterrupt:ExternalInterrupt:::IndexType::EINT1;

#elif  defined ARDUINO_UNO || defined MYAVR_BOARD_MK2

    #include <target/MYAVR_BOARD_MK2/Interrupt.hpp>     // Target specific interrupt header

    constexpr ExternalInterrupt::IndexType EINT0 = ExternalInterrupt::IndexType::INT0;
    constexpr ExternalInterrupt::IndexType EINT1 = ExternalInterrupt::IndexType::INT1;

#elif defined STM32F407VG

    #include <target/STM32F407VG/Interrupt.hpp>         // Target specific interrupt header

    constexpr ExternalInterrupt::IndexType EINT0 = ExternalInterrupt::IndexType::EXTI0_PA0;
    constexpr ExternalInterrupt::IndexType EINT1 = ExternalInterrupt::IndexType::EXTI3_PB3;

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
    
    constexpr TriggerType EDGE_FALLING = TriggerType::EDGE_FALLING;
    constexpr TriggerType EDGE_RISING = TriggerType::EDGE_RISING;


    InitGPIO();         // Initialize Power, LED and Pushbutton

    
    ExternalInterrupt exti0_cfg = {EINT0, EDGE_RISING}; 
    ExternalInterrupt exti1_cfg = {EINT1, EDGE_FALLING};

    exti0_cfg.apply();
    exti1_cfg.apply();

    auto& vectorTable = InterruptVectorTable::getInstance();    // move vector table into singleton/RAM/ aligned memory block
    vectorTable.setCallback(IRQType::EXTI0_IRQn, PushButton_Handler);
    vectorTable.setCallback(IRQType::EXTI3_IRQn, PushButton_Handler);
    
    vectorTable.enableISR(IRQType::EXTI0_IRQn);
    vectorTable.enableISR(IRQType::EXTI3_IRQn);

    vectorTable.enableIRQ();

    while(1)
    {
        // delay_ms(500);
        // vectorTable.triggerIRQ(IRQIndex);
    } 
}


#ifdef __cplusplus
}
#endif