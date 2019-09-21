#include <LPC17xx/LPC17xx.h> 

#ifdef __cplusplus
extern "C" {
#endif

void delay_ms(unsigned int ms)
{
unsigned int i,j;
for(i=0;i<ms;i++)
for(j=0;j<6000;j++);
}


// main() runs in its own thread in the OS
int main() {
    SystemInit();
    
    //resetBits<uint32_t>(&(LPC_PINCON->PINSEL3), {24, 25});
    LPC_PINCON->PINSEL4 = 0x00000000;  //Configure the PORT2 Pins as GPIO;
    LPC_GPIO2->FIODIR = 0xffffffff;

    
    while(true)
    {
        LPC_GPIO2->FIOSET = 0xffffffff; // Make all the Port pins as high  
        delay_ms(200);
        LPC_GPIO2->FIOCLR = 0xffffffff;     // Make all the Port pins as low  
        delay_ms(200);
    }
}


#ifdef __cplusplus
}
#endif
 
 