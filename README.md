### Using platformIO on macOS with VS Code and the LPC1768 board by KEIL and the ULINK-ME JTAG adapter
A working platformIO blinky example for the LPC1768 board by KEIL.  
This was constructed with the help of [maxgerhardt](https://github.com/maxgerhardt) in this forum post [platformIO community forum](https://community.platformio.org/t/how-to-properly-set-the-upload-port-on-macos-lpc1768-board-ulink-me/9560/16).



### Troubleshooting

#### LPC17xx chipset not correctly identified.
Checkout this commit, as it's the latest that supports the LPC17xx chipset correctly.
```
Firmware found: LPC17xx
88ec55768389c6c4de69ecd6e101f6b136c25a8f
```

First commit that doesn't find the correct firmware
```
Firmware found: Nordic nRF51 (might be missing stuff behind the name later on Nordic nRF51 M3/M4 is found)
302ff20a6d5b806c09e0ca7e996beab3ef3596f4
```

#### Remove any other installed platformIO or openOCD installation
```
brew remove platformio open-ocd
```

#### Python Module 'intelhex' not found
```
cd
./.platformio/penv/bin/pip3 install intelhex
```

#### Error: registers may not be the same -- `strexb r0,r0,[r1]' `strexh r0,r0,[r1]'

Use this patch: [click me](https://gist.github.com/timbrom/1942280)


### Mbed Blinky example code
```c++
#include "mbed.h"
 
DigitalOut led1(LED1);
 
// main() runs in its own thread in the OS
int main() {
    while (true) {
        led1 = !led1;
        wait(0.5);
    }
}
```