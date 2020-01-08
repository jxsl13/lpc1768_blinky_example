
#if defined MCB1760

    #include <target/MCB1760/Interrupt.hpp>             // Target specific interrupt header

    constexpr ExternalInterrupt::IndexType EINT0 = ExternalInterrupt::IndexType::EINT0_P2_10;
    constexpr ExternalInterrupt::IndexType EINT1 = ExternalInterrupt::IndexType::EINT1_P2_11;
    constexpr ExternalInterrupt::TriggerType EDGE = ExternalInterrupt::TriggerType::EDGE_FALLING;

    constexpr IRQType IRQ_EINT0 = IRQType::EINT0_IRQn;
    constexpr IRQType IRQ_EINT1 = IRQType::EINT1_IRQn;

    constexpr IRQType IRQ_WDT = IRQType::WDT_IRQn;

#elif  defined ARDUINO_UNO || defined MYAVR_BOARD_MK2

    #include <target/MYAVR_BOARD_MK2/Interrupt.hpp>     // Target specific interrupt header

    constexpr ExternalInterrupt::IndexType EINT0 = ExternalInterrupt::IndexType::EINT0_PD2;
    constexpr ExternalInterrupt::IndexType EINT1 = ExternalInterrupt::IndexType::EINT1_PD3;
    constexpr ExternalInterrupt::TriggerType EDGE = ExternalInterrupt::TriggerType::EDGE_FALLING;

    constexpr IRQType IRQ_EINT0 = IRQType::INT0_IRQn;
    constexpr IRQType IRQ_EINT1 = IRQType::INT1_IRQn;

    constexpr IRQType IRQ_WDT = IRQType::WDT_IRQn;

#elif defined STM32F407VG

    #include <target/STM32F407VG/Interrupt.hpp>         // Target specific interrupt header

    constexpr ExternalInterrupt::IndexType EINT0 = ExternalInterrupt::IndexType::EXTI0_PA0;
    constexpr ExternalInterrupt::IndexType EINT1 = ExternalInterrupt::IndexType::EXTI3_PB3;
    constexpr ExternalInterrupt::TriggerType EDGE = ExternalInterrupt::TriggerType::EDGE_RISING;

    constexpr IRQType IRQ_EINT0 = IRQType::EXTI0_IRQn;
    constexpr IRQType IRQ_EINT1 = IRQType::EXTI3_IRQn;

    constexpr IRQType IRQ_WDT = IRQType::WWDG_IRQn;

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
extern void ToggleLED();
extern void EnableLED();
extern void DisableLED();

extern void InitGPIO();
extern void InitWatchDogTimer();

// clears pushbutton pending flag
extern void ClearIRQCondition();



void Blinking(unsigned int times = 5, unsigned int ms = 300)
{
    for (ValueType i = 0; i < times * 2; i++)
    {
        delay_ms(ms);
        ToggleLED();
    } 
}

void WatchDogInterrupt_Handler()
{
    //FeedWatchDog();
    ToggleLED();
}


ValueType gTestCase = 0;

void PushButton_Handler()
{
    ClearIRQCondition();
    gTestCase++;
    //Blinking(gTestCase, 500);
}


void Reset()
{
    gTestCase = 0;
}


using IndexType = ExternalInterrupt::IndexType;
using TriggerType = ExternalInterrupt::TriggerType;

int main()
{   

    // constexpr TriggerType EDGE_FALLING = TriggerType::EDGE_FALLING;
    // constexpr TriggerType EDGE_RISING = TriggerType::EDGE_RISING;

    InitGPIO(); // Initialize Clock, Power, LED and Pushbutton
    DisableLED();

    ExternalInterrupt exti0Cfg = {EINT0, EDGE}; 
    exti0Cfg.apply();   // apply configuration

    auto& vectorTable = InterruptVectorTable::getInstance(); // move vector table into singleton/RAM/ aligned memory block
    vectorTable.setCallback(IRQ_EINT0, PushButton_Handler); // enable external interrupt service routines
    vectorTable.enableISR(IRQ_EINT0);

                     
    //ExternalInterrupt exti1Cfg = {EINT1, EDGE_FALLING};        
    //exti1Cfg.apply();   // apply configuration     
    //vectorTable.setCallback(IRQ_EINT1, PushButton_Handler);    
    //vectorTable.enableISR(IRQ_EINT1);

    // enable global interrupts
    vectorTable.enableIRQ(); 


    while(1)
    {
        switch (gTestCase) 
        {
            case 0:
                vectorTable.waitForIRQ();
                Blinking(gTestCase, 500);

                delay_ms(500); // delay in order to trigger more interrupts
                break;
            case 1:
                // this case resets potential inintentionally triggered external interrupts on startup
                Reset();
                break;
            case 2:
                vectorTable.disableIRQ();
                
                EnableLED(); // should be turned off, when the system restarts

                InitWatchDogTimer();    // Initialize Watchdog Timer, 4 seconds prescaler, only triggers ISR

                // enable WDT interrupt service routine
                vectorTable.setCallback(IRQ_WDT, WatchDogInterrupt_Handler);
                vectorTable.enableISR(IRQ_WDT);

                // enable global interrupts
                vectorTable.enableIRQ();
                
                while (1);

                break;

            default:
                Reset();
                break;
        }
    } 
}


#ifdef __cplusplus
}
#endif