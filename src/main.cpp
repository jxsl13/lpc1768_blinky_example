#include <controllers/atmega328p/holmes_platform.hpp>     // Target specific interrupt header




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

void PushButton_Handler()
{
    ToggleLED();
    ClearIRQCondition();
}



void Blinking(unsigned int times = 5, unsigned int ms = 300)
{
    for (unsigned int i = 0; i < times * 2; i++)
    {
        delay_ms(ms);
        ToggleLED();
    } 
}








int main()
{   
    using IRQType = holmes::IRQType;
    constexpr IRQType IRQ_EINT0 = IRQType::INT0_IRQn;

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
        Blinking(4, 500);               
    } 
}

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif