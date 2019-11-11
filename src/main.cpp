#ifdef __cplusplus
extern "C" {
#endif

#include <hal/InterruptVectorTable.hpp>
#include <config.hpp>


#if defined MCB1760

// MCB1760 supports the STL, AVR Doesn't!
#include <cstring>
#include <array>


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

inline void ClearInterruptFlagEINTX(uint32_t X)
{
    LPC_SC->EXTINT |= (1<<X);      // Clear Interrupt Flag
}

void PushButton_Handler()
{
    ClearInterruptFlagEINTX(0);
    ToggleLED(); 
}

void InitRAMInterruptVectorTable()
{
    constexpr uint32_t VECTORTABLE_SIZE = 256;

    /**
     * @brief Aligment is in bytes!
     *  Global static vector table variable
     * 
     * compiler seems to align correctly in any case.
    */
    alignas(VECTORTABLE_SIZE * sizeof(uint32_t)) static std::array<uint32_t, VECTORTABLE_SIZE> g_VectorTable;

    uint32_t *vectors = (uint32_t *)SCB->VTOR;

    // copy vector table to ram location
    std::memcpy(g_VectorTable.data(), vectors, sizeof(uint32_t) * VECTORTABLE_SIZE);


    /* relocate vector table into RAM*/ 
    // disable global interrupts
    __disable_irq();

    // change vectortable location
    SCB->VTOR = (uint32_t) g_VectorTable.data();
    
    // wait for memory operations to finish
    __DSB();

    // enable interrupts again.
    __enable_irq();
}

void InitEINT0()
{
    LPC_SC->EXTINT      = (1<<0);	    // Clear Pending interrupts
    LPC_SC->EXTMODE     = (1<<0);       // Configure EINTx as Edge Triggered
    LPC_SC->EXTPOLAR    = (1<<0);       // Configure EINTx as Falling Edge
}

void BindEINT0Handler()
{
    NVIC_SetVector((IRQn)(18), (uint32_t)PushButton_Handler);
}

void EnableEINT0()
{
    // EINT0 = 18
    NVIC_EnableIRQ((IRQn_Type)(18));    /* Enable the EINT0 interrupt */
}

void FireInterrupt(uint32_t InterruptIndex)
{
    if (InterruptIndex < 0 || InterruptIndex > 111)
        return;
    
    /**
     * Bits 8:0 - 256 Interrupt values
     * Values of bits 31:9 are reserved, so should not be read or touched
     * Manual UM10360 - Page 92
     */
    NVIC->STIR |= (0xFF & InterruptIndex);
}

int main()
{   
    SystemInit();       // startup board in a predefined state

    InitPower();        // explicitly enable power
    InitPushButton();   // configure pushbutton to trigger the EINT0 interrupt
    InitLED();          // configure LED's GPIO pin to be an output & set its value to LOW

    // switch between the encapsulated and non encapsulated variants
    constexpr bool useEncapsulated = true;
    if(useEncapsulated)
    {
        InitEINT0();                                                // init eint0
        auto& vectorTable = InterruptVectorTable::getInstance();    // move vector table into singleton/RAM/ aligned memory block

        // tell the controller to use the PushButton_Handler when the interrupt 18 = EINT0 is triggered
        vectorTable.setCallback(18, PushButton_Handler);

        // allow the EINT0 inrettupt to be triggered
        vectorTable.enableISR(18);

        while(1)
        {
            // trigger interrupt via software (easier testing, less configuration of buttons etc.)
            vectorTable.triggerIRQ(18);
            delay_ms(1000);
        }   
    }
    else
    {
        InitRAMInterruptVectorTable();  // move vector table to ram
        InitEINT0();                    // init eint0
        BindEINT0Handler();             // make EINT0 execute PushButtonHandler on interrupt
        EnableEINT0();                  // enable interrupt and bind to handler

        while (1)
        {
            FireInterrupt(18);
            delay_ms(500);
        }
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
    using irqt = InterruptVectorTable::IRQTypes;
    auto& VectorTable = InterruptVectorTable::getInstance();

    InitGPIO();
    InitINT0();

    VectorTable.setCallback(irqt::INT0_IRQn, [](){if(EIFR & (1 << 0)) ToggleLED();});
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
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;    // Enable Clock for Port A
    GPIOA->MODER |= 0x0;                    // PA0 is the input(?) for the user pushbutton.



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
    ValueType Trigger = Trigger_Falling_Edge;

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
    //ENABLE(EXTI->EMR, EXTIX);  // EventMaskRegister ->     EXTI_EMR_MR0 = 0

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

void ToggleLED()
{
    // PD12 -> LED Pin
    GPIOD->ODR ^= (1<<12); 		// toggle diodes
}

void EXTI0_IRQHandler()
{
    ToggleLED();
}

int main(void)
{
    InitGPIO();
    InitEXTI0();

    while (1)
    {
        ms_delay(500);
        ToggleLED();
    }

}

#endif // Different mains


#ifdef __cplusplus
}
#endif