#include "mbed.h"

/** 
 * does not necessarily work, but enough for a overhead demonstration
 */

#if defined LPC1768
    constexpr PinName LED = PinName::P1_28;
    constexpr PinName BUTTON = PinName::P2_10;
#elif defined STM32F407VG
    constexpr PinName LED = PinName::PD_12;
    constexpr PinName BUTTON = PinName::PA_0;
#endif
 
DigitalOut led1(LED);
InterruptIn btn(BUTTON);
 

void blink_led1() {
    led1 = !led1;
}
 
 
int main() {
    btn.fall(callback(&blink_led1));
    btn.enable_irq();

    while(1) {}
}