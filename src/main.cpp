

#include <hal/InterruptVectorTable.hpp>
#include <config.hpp>

#ifdef __cplusplus
extern "C" {
#endif

#if defined MCB1760


void delay_ms(unsigned int ms)
{
    volatile unsigned int i,j;
    for(i=0;i<ms;i++)
    for(j=0;j<6000;j++);
}


void InitGPIO()
{   
    SystemInit();                       // startup board in a predefined state

    /* Enable Power */
    ENABLE(LPC_SC->PCONP, 15);          // Enable power

    /* Init LED */
    DISABLE(LPC_PINCON->PINSEL3, 24);   // LED connected to P1.28 is in GPIO mode (see Table 83)
    DISABLE(LPC_PINCON->PINSEL3, 25);   // second bit of P1.28

    ENABLE(LPC_GPIO1->FIODIR, 28);      // Configure LED pins as OUTPUT - P1.28
    ENABLE(LPC_GPIO1->FIOCLR, 28);      // set P1.28 to LOW

    /* Init Pushbutton */
    ENABLE(LPC_PINCON->PINSEL4, 20);    // Configure P2_10, as EINT0 (see data sheet of PINSEL4)
    DISABLE(LPC_GPIO2->FIODIR, 10);     // P2.10 is an input pin
    ENABLE(LPC_GPIOINT->IO2IntEnF, 10); // P2.10 reads the falling edges to generate an interrupt
}

void InitExtInt0()
{
    ENABLE(LPC_SC->EXTINT, 0);      // Clear Pending interrupts
    ENABLE(LPC_SC->EXTMODE, 0);     // Configure EINTx as Edge Triggered
    ENABLE(LPC_SC->EXTPOLAR, 0);    // Configure EINTx as Falling Edge      
}

void ToggleLED()
{
    TOGGLE(LPC_GPIO1->FIOPIN, 28);
}

void ClearIRQCondition()
{
    ENABLE(LPC_SC->EXTINT, 0); // Clear Interrupt Flag/Pending Bit -> 0 = EINT0
}

void PushButton_Handler()
{
    ToggleLED();
    ClearIRQCondition();
}



int main()
{   
    ValueType IRQIndex = 18;
    InitGPIO();         // Initialize Power, LED and Pushbutton
    InitExtInt0();      // configure, how the interrupt is triggered(EINT0).

    auto& vectorTable = InterruptVectorTable::getInstance();    // move vector table into singleton/RAM/ aligned memory block
    vectorTable.setCallback(IRQIndex, PushButton_Handler);
    vectorTable.enableISR(IRQIndex);
    vectorTable.enableIRQ();

    while(1)
    {
        vectorTable.triggerIRQ(IRQIndex);
        delay_ms(1000);
    } 
}

#elif defined ARDUINO_UNO || defined MYAVR_BOARD_MK2

void delay_ms(unsigned int ms)
{
    while (0 < ms)
    {  
        _delay_ms(1);
        --ms;
    }
}

void InitGPIO()
{
    // make port output & HIGH
    // our power source
    ENABLE(DDRB, DDB4);
    ENABLE(PORTB, PORTB4);

    // make Port B5 output & LOW
    // Connected to an LED - Depending on the state of this, 
    // the LED is either on or off.
    ENABLE(DDRB, DDB5);
    DISABLE(PORTB, PORTB5);

    // Make Port D2 Input External Interrupt
    DISABLE(DDRD, DDD2);
    ENABLE(PORTD, PORTD2);
}

void InitExtInt0()
{
    // Manual - Page 80
    // The falling edge of INT0 generates an interrupt request.
    ENABLE(EICRA, ISC01);
    DISABLE(EICRA, ISC00);
}

void ToggleLED()
{
    // toggle the LED
    TOGGLE(PORTB, PORTB5);    
}

void PushButton_Handler()
{
    
    ToggleLED();
    
    
    ENABLE(EIFR, 0); // clear more or less pending bit.
    
    
}


int main(void)
{
    ValueType IRQIndex = 1;

    InitGPIO();     // Configure LED(IN), Pushbutton(OUT)
    InitExtInt0();  // INT0 Edge triggered

   
    auto& VectorTable = InterruptVectorTable::getInstance();
    VectorTable.setCallback(IRQIndex, PushButton_Handler);
    VectorTable.enableISR(IRQIndex);
    VectorTable.enableIRQ();

    while (1)
    {   
        VectorTable.triggerIRQ(IRQIndex);
        delay_ms(1000);
    }
}

#elif defined STM32F407VG



void delay_ms(unsigned int ms)
{
   while (ms-- > 0) {
      volatile int x=6000;
      while (x-- > 0)
         __asm("nop");
   }
}

enum PortX : ValueType
{
        PA = 0,
        PB = 1,
        PC = 2,
        PD = 3,
        PE = 4,
        PF = 5,
        PG = 6,
        PH = 7,
        PI = 8,
};

enum Trigger : ValueType
{
    Trigger_None = 0b00,
    Trigger_Rising_Edge = 0b01,             
    Trigger_Falling_Edge = 0b10,           
    Trigger_Logical_Change = Trigger_Rising_Edge | Trigger_Falling_Edge,
};

void InitGPIO()
{

    // LED
    ValueType PD12 = 12;
    ENABLE(RCC->AHB1ENR, PD);               // enable clock on Port D
    ENABLE(GPIOD->MODER, (2 * PD12));       // 0b01 -> General Purpose ouput
    DISABLE(GPIOD->MODER, (2 * PD12) + 1);  // set PD12 to Output

    // PA0 - External Input
    ValueType PA0 = 0;
    ENABLE(RCC->AHB1ENR, PA);           // Enable Clock for Port A
    DISABLE(GPIOA->MODER, 2 * PA0);     // set PA0 to be an input
    DISABLE(GPIOA->MODER, 2 * PA0 + 1); // PA0 input
    

    // Trigger EXTI0 with PA0 (EXTI[X] -> P[Y][X])
    ValueType Port = PA;
    ValueType EXTIX = 0;

    /* 
        P[X] will trigger the interrupt
        let Port be A and EXTIX be 1,
        this will lead to PA1 to trigger the interupt EXTI1
    */
    SYSCFG->EXTICR[EXTIX % 4] |= (Port << (EXTIX * 4));
}


void InitEXTI0()
{

    ValueType EXTIX = 0;
    ValueType Trigger = Trigger_Rising_Edge;

    if (!Trigger)
        return;
    
    if (EXTIX > 22)
        return;
    
    /* Clear EXTI line configuration */
    /**
     * LINE := (1 << Line_Number)
     * 
     * We trigger events and interrupts at the same time!
     */
    ENABLE(EXTI->IMR, EXTIX);  // InterruptMaskRegister -> EXTI_IMR_MR0 = 0
    ENABLE(EXTI->EMR, EXTIX);  // EventMaskRegister ->     EXTI_EMR_MR0 = 0

    if (Trigger == Trigger_Rising_Edge)
    {
        ENABLE(EXTI->RTSR, EXTIX);
        DISABLE(EXTI->FTSR, EXTIX);
    }
    else if(Trigger == Trigger_Falling_Edge)
    {
        DISABLE(EXTI->RTSR, EXTIX);
        ENABLE(EXTI->FTSR, EXTIX);
    }
    else
    {
        ENABLE(EXTI->RTSR, EXTIX);
        ENABLE(EXTI->FTSR, EXTIX);
    }
}

void ClearIRQCondition()
{
    ValueType IRQIndex = EXTI0_IRQn;

    if (IRQIndex > 22)
    {
        //EXTI->PR |= ((1 << IRQIndex) & 0x7FFFFF); // first [22:0]
        return; // mask prevents any other bits from being set.
    }

    ENABLE(EXTI->PR, IRQIndex);    
}


void ToggleLED()
{
    ValueType PD12 = 12;
    TOGGLE(GPIOD->ODR, PD12);
}

void Pushbutton_Handler()
{
    ToggleLED();
    // clear interrupt condition (EXTI Pending Bit Register)
    // otherwise you will get tailchained interrupts.
    ClearIRQCondition();
}



int main(void)
{
    ValueType IRQIndex = 6;

    InitGPIO();
    InitEXTI0();

    auto& VectorTable = InterruptVectorTable::getInstance();
    VectorTable.setCallback(IRQIndex, ToggleLED);
    
    VectorTable.enableISR(IRQIndex);
    VectorTable.enableIRQ();    
    

    while (1)
    {
        
        VectorTable.triggerIRQ(IRQIndex);
        delay_ms(1000);
    }

}

#endif // Different mains


#ifdef __cplusplus
}
#endif