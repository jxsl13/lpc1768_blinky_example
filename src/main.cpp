#include "proxy.h"

#ifdef __cplusplus
extern "C" {
#endif

void delay_ms(unsigned int ms)
{
volatile unsigned int i,j;
for(i=0;i<ms;i++)
for(j=0;j<100000;j++);
}


/* start the main program */
int main() 
{
    
    while(1)
    {
        delay_ms(1000);
        delay_ms(1000);
    }
}


#ifdef __cplusplus
}
#endif
 
 