#include "proxy.h"

#ifdef __cplusplus
extern "C" {
#endif

void delay_ms(unsigned int ms)
{
volatile unsigned int i,j;
for(i=0;i<ms;i++)
for(j=0;j<10000;j++);
}


/* start the main program */
int main() 
{
    SystemInit();                    //Clock and PLL configuration 
    LPC_PINCON->PINSEL4 = 0x00000000;//Configure the PORT2 Pins as GPIO;
    LPC_GPIO2->FIODIR = 0xffffffff;  //Configure the PORT2 pins as OUTPUT;

    LPC_GPIO2->FIOPIN = 0x00000000;      // Make all the Port pins as low  
    delay_ms(1000);

    //LPC_GPIO2->FIOPIN = 0xffffffff;     // Make all the Port pins as high  
    //delay_ms(1000);

    while(1)
    {
        //LPC_GPIO2->FIOPIN = 0x00000000;      // Make all the Port pins as low  
        //delay_ms(1000);

        //LPC_GPIO2->FIOPIN = 0xffffffff;     // Make all the Port pins as high  
        //delay_ms(1000);
    }
}


#ifdef __cplusplus
}
#endif