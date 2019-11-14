

#include <hal/InterruptVectorTable.hpp>
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


int main()
{   
    ValueType IRQIndex = IRQ_INDEX;
    InitGPIO();         // Initialize Power, LED and Pushbutton
    InitExtInt0();      // configure, how the interrupt is triggered(EINT0).

    auto& vectorTable = InterruptVectorTable::getInstance();    // move vector table into singleton/RAM/ aligned memory block
    vectorTable.setCallback(IRQIndex, PushButton_Handler);
    vectorTable.enableISR(IRQIndex);
    vectorTable.enableIRQ();

    while(1)
    {
        vectorTable.triggerIRQ(IRQIndex);
        delay_ms(1000);
    } 
}


#ifdef __cplusplus
}
#endif