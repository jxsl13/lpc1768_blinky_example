#include "mbed.h"

// disable these LEDs
DigitalOut led2(P1_29, 0);
DigitalOut led3(P1_31, 0);
DigitalOut led4(P2_2, 0);
DigitalOut led5(P2_3, 0);
DigitalOut led6(P2_4, 0);
DigitalOut led7(P2_5, 0);
DigitalOut led8(P2_6, 0);

// only blink this LED
DigitalOut myled(P1_28);

int main() {
    while(1) {
        myled = 1;
        wait(1);
        myled = 0;
        wait(1);
    }
}