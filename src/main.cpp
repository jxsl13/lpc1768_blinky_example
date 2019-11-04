#include "hal/InterruptVectorTable.hpp"
#include "proxy.hpp"

#include <cstring>
#include <array>

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
    NVIC_SetVector((IRQn)(18), (uint32_t)PushButton_Handler); // TODO: get this to work
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
        vectorTable.addCallback(18, PushButton_Handler);

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

void InitLED()
{
    // make the LED pin an output for PORTB5
    // Port B, Bit 5-> B5
    DDRB = 1 << DDB5;       // DDB5 = 5
}

void ToggleLED()
{
    // toggle the LED
    PORTB ^= 1 << PB5;  // PB5 = 5
}

static std::array<std::vector<std::reference_wrapper<uint8_t>>, 25> g_InterruptVectorTable;
int main(void)
{
    uint8_t *vectors = (uint8_t *)0x0;

   for (uint8_t i = 0; i < 25; i++)
   {
       g_InterruptVectorTable[i] = vectors[i];
   }
   
   InitLED();
    while (1)
    {
        _delay_ms(200);
        ToggleLED();
    }

    return 0;
}

#elif defined STM32F407VG

#define LEDPORT (GPIOD)
#define LED1 (12)
#define ENABLE_GPIO_CLOCK (RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN)
#define GPIOMODER (GPIO_MODER_MODER12_0)


void ms_delay(int ms)
{
   while (ms-- > 0) {
      volatile int x=1000;
      while (x-- > 0)
         __asm("nop");
   }
}

int main(void)
{
	ENABLE_GPIO_CLOCK; 		 			// enable the clock to GPIO
	LEDPORT->MODER |= GPIOMODER;		// set pins to be general purpose output
	for (;;) {
		ms_delay(1000);
		LEDPORT->ODR ^= (1<<LED1); 		// toggle diodes
	}
	
	return 0;
}

#endif // Different mains


#ifdef __cplusplus
}
#endif