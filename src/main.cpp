

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

void InitPower()
{
    LPC_SC->PCONP |= (1 << 15);             // Enable power
}

void InitLED()
{
    LPC_PINCON->PINSEL3 &=   ~(0 << 25) | ~(0 << 24);       // LED connected to P1.28 is in GPIO mode (see Table 83)
    LPC_GPIO1->FIODIR   =                   (1<< 28);       // Configure LED pins as OUTPUT - P1.28*/
    LPC_GPIO1->FIOCLR   |=                 (1 << 28);       // set P1.28 to LOW
}

void InitPushButton()
{

    LPC_PINCON->PINSEL4     =     (1<<20);      // Configure P2_10, as EINT0 (see data sheet of PINSEL4)
    LPC_GPIO2->FIODIR       &= ~(0 << 10);      // P2.10 is an input pin
    LPC_GPIOINT->IO2IntEnF  |=  (1 << 10);      // P2.10 reads the falling edges to generate an interrupt
}

void ToggleLED()
{
    #define CHECK_BIT(var,pos) ((var) & (1<<(pos)))

    // check if bit is set, then toggle it depending on result.
    if(CHECK_BIT(LPC_GPIO1->FIOPIN, 28))
    {
        LPC_GPIO1->FIOCLR |= (1 << 28);
    }
    else
    {
        LPC_GPIO1->FIOSET |= (1 << 28);
    }
}

// TODO: also needs to have the pending bit cleared.
inline void ClearInterruptFlagEINTX(ValueType X)
{
    ENABLE(LPC_SC->EXTINT, X);     // Clear Interrupt Flag/Pending Bit
}

void PushButton_Handler()
{
    ToggleLED();
    ClearInterruptFlagEINTX(0);
}


void InitEINT0()
{
    LPC_SC->EXTINT      = (1<<0);	    // Clear Pending interrupts
    LPC_SC->EXTMODE     = (1<<0);       // Configure EINTx as Edge Triggered
    LPC_SC->EXTPOLAR    = (1<<0);       // Configure EINTx as Falling Edge
}


int main()
{   
    ValueType IQRNumber = 18;
    SystemInit();       // startup board in a predefined state

    InitPower();        // explicitly enable power
    InitPushButton();   // configure pushbutton to trigger the EINT0 interrupt
    InitLED();          // configure LED's GPIO pin to be an output & set its value to LOW
    InitEINT0();        // configure, how the interrupt is triggered.

    auto& vectorTable = InterruptVectorTable::getInstance();    // move vector table into singleton/RAM/ aligned memory block

    // tell the controller to use the PushButton_Handler when the interrupt 18 = EINT0 is triggered
    vectorTable.setCallback(IQRNumber, PushButton_Handler);

    // allow the EINT0 inrettupt to be triggered
    vectorTable.enableISR(IQRNumber);

    // enable global interrupts
    vectorTable.enableIRQ();

    while(1)
    {
        // trigger interrupt via software (easier testing, less configuration of buttons etc.)
        vectorTable.triggerIRQ(IQRNumber);
        delay_ms(1000);
    } 
}

#elif defined ARDUINO_UNO || defined MYAVR_BOARD_MK2


void ToggleLED()
{
    // toggle the LED
    TOGGLE(PORTB, PORTB5);
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

void InitINT0()
{
    // external interrupt 0

    // Manual - Page 80
    // The falling edge of INT0 generates an interrupt request.
    ENABLE(EICRA, ISC01);
    DISABLE(EICRA, ISC00);
}



int main(void)
{
    auto& VectorTable = InterruptVectorTable::getInstance();
    ValueType INT0_IRQn = 1;

    InitGPIO();
    InitINT0();

    VectorTable.setCallback(INT0_IRQn, 
        []() -> void {
            if(EIFR & (1 << 0)) ToggleLED();
        }
    );
    VectorTable.enableISR(1);
    VectorTable.enableIRQ();

    while (1)
    {   
    }

    return 0;
}

#elif defined STM32F407VG



void ms_delay(int ms)
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
    Trigger_None = 0,
    Trigger_Rising_Edge = 1,             
    Trigger_Falling_Edge = 2,           
    Trigger_Logical_Change = 3,
};

void InitGPIO()
{

    // LED
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN; // enable clock on port D
    GPIOD->MODER |= GPIO_MODER_MODER12_0; // _0 -> 0b01 -> General Purpose ouput

    // PA0 - External Input
    ValueType EXTIPin = 0;

    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;        // Enable Clock for Port A
    GPIOA->MODER &= ~(0x03 << (2 * EXTIPin));   // PA0 is the input(?) for the user pushbutton.
    


    // Trigger EXTI0 with PA0 (EXTI[X] -> P[Y][X])
    ValueType Port = PortX::PA;
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

void ClearEXTI0Condition()
{
    ENABLE(EXTI->PR, EXTI0_IRQn);
}

void ToggleLED()
{
    ValueType PD12 = 12;
    
    // PD12 -> LED Pin
    TOGGLE(GPIOD->ODR, PD12);
    
    // clear interrupt condition (EXTI Pending Bit Register)
    // otherwise you will get tailchained interrupts.
    ClearEXTI0Condition();
}



void FastBlinking()
{
    for (ValueType i = 0; i < 16; i++)
    {
        ToggleLED();
        ms_delay(100);
    }    
}

void SlowBlinking()
{
    for (ValueType i = 0; i < 8; i++)
    {
        ToggleLED();
        ms_delay(200);
    }
}

void EXTI0_IRQHandler()
{
    FastBlinking();

    constexpr ValueType IRQIndex = 6;


    // clear pending bit, otherwise the interrupt is being fired indefinitly.
    // don't touch bits [31:23]
    ValueType active  = !NVIC_GetActive((IRQn_Type)IRQIndex);
    if (active)
    {
        FastBlinking();
        NVIC_ClearPendingIRQ((IRQn_Type)IRQIndex);
    }
    
    EXTI->PR |= ((1 << IRQIndex) & 0x7FFFFF);
    
}

void EnableInterrupts()
{
    __enable_irq();
}

int main(void)
{
    ValueType IRQIndex = 6;

    InitGPIO();
    InitEXTI0();

    auto& VectorTable = InterruptVectorTable::getInstance();
    VectorTable.setCallback(IRQIndex, ToggleLED);
    
    VectorTable.enableISR(IRQIndex);
    NVIC_EnableIRQ((IRQn_Type)IRQIndex);
    EnableInterrupts();
    
    

    while (1)
    {
        ms_delay(1000);
        NVIC->STIR |= (0x1FF & IRQIndex);
    }

}

#endif // Different mains


#ifdef __cplusplus
}
#endif