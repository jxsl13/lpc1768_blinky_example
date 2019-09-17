#include "mbed.h"
 
DigitalOut led1(P1_28);
 
// main() runs in its own thread in the OS
int main() {
    while (true) {
        led1 = !led1;
        wait(0.5);
    }
}
 
 