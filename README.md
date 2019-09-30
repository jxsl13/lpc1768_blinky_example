### This is a multi target abstraction of an interrupt configuration.

#### Current boards:
 - Keil MCB1760 (LPC1768 ARM Cortex M-3 Microcontroller)
 - myAVR Board MK2 V2.20(ATmega328p microcontroller)
 - Arduino UNO(ATmega328p microcontroller)

#### Target specific READMEs
 - `src/target/LPC1768/README.md`
 - `src/target/ATMEGA328P/README.md`

### Troubleshooting  

#### LPC1768 Debugging probe/Programmer not recognized or throws some weird errors.
First power on the Keil MCB1760 board via USB. Then attach the ribbon cable or the debugging probe to your board and lastly power on your debugging probe by connecting it via USB as well.

#### LPC17xx chipset not correctly identified by Blackmagic Probe.
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

