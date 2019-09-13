### Using platformIO on macOS with VS Code and the LPC1768 board by KEIL and the ULINK-ME JTAG adapter
A working platformIO blinky example for the LPC1768 board by KEIL.  
This was constructed with the help of [maxgerhardt](https://github.com/maxgerhardt) in this forum post [platformIO community forum](https://community.platformio.org/t/how-to-properly-set-the-upload-port-on-macos-lpc1768-board-ulink-me/9560/16).



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