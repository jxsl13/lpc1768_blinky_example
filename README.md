### Using platformIO on macOS with VS Code and the LPC1768 board by KEIL and the ULINK-ME JTAG adapter
A working platformIO blinky example for the LPC1768 board by KEIL.  
This was constructed with the help of [maxgerhardt](https://github.com/maxgerhardt) in this forum post [platformIO community forum](https://community.platformio.org/t/how-to-properly-set-the-upload-port-on-macos-lpc1768-board-ulink-me/9560/16).

### Using the Blackmagic Probe
The Blackmagic Probe claims support, but might have some setup issues. See below.

### Some basic commands

```
// start debugger
~/.platformio/packages/toolchain-gccarmnoneeabi/bin/arm-none-eabi-gdb

// connect to Blackmagic Probe (your device might be a different one)
(gdb) target extended-remote /dev/cu.usbmodem79AC68971

// use swd interface (alternatively jtag_scan)
(gdb) monitor swdp_scan

// attach to that interface
(gdb) attach 1

// use this file for debugging
(gdb) file ~/Documents/PlatformIO/Projects/<YOUR PROJECT FOLDER>/.pio/build/lpc1768/firmware.elf

// upload the file
(gdb) load

// start execution of the flashed firmware
(gdb) start
```



### Troubleshooting

#### Remove any other installed platformIO or openOCD installation
```
brew remove platformio open-ocd
```

#### Python Module 'intelhex' not found
```
./.platformio/penv/bin/pip3 install intelhex
```

#### Error: registers may not be the same -- `strexb r0,r0,[r1]' `strexh r0,r0,[r1]'

Use this patch: [click me](https://gist.github.com/timbrom/1942280)

#### Blackmagic Probe doing weird stuff
First connect the BMP to the board via the ribbon cable. Then connect it to your USB port.  
The orange LED should be lit indicating that everything seems to work fine.
Downgrade your BMP firmware to an earlier version.
Build the firmware with `make all`. You will find a `blackmagic.bin` file in the src folder.




