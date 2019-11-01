#include "hal/InterruptVectorTable.hpp"
#include "proxy.hpp"


#include <cstring>
#include <array>

#ifdef __cplusplus
extern "C" {
#endif


#if defined MCB1760


#define PINSEL_EINT0    20
#define LED1            28   
#define SBIT_EINT0      0
#define SBIT_EXTMODE0   0
#define SBIT_EXTPOLAR0  0

void delay_ms(unsigned int ms)
{
    volatile unsigned int i,j;
    for(i=0;i<ms;i++)
    for(j=0;j<6000;j++);
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
    /* Clear Interrupt Flag */
    LPC_SC->EXTINT |= (1<<X);  
}


void InitPower()
{
    LPC_SC->PCONP |= (1 << 15);             // Enable power
}

void InitLED()
{
    LPC_PINCON->PINSEL3 &= ~(0 << 25) | ~(0 << 24);   // LED connected to P1.28 is in GPIO mode (see Table 83)
    LPC_GPIO1->FIODIR   = (1<< 28);                   // Configure LED pins as OUTPUT - P1.28*/

    LPC_GPIO1->FIOCLR |= (1 << 28); // set P1.28 to LOW
}

void InitPushButton()
{

    LPC_PINCON->PINSEL4 = (1<<PINSEL_EINT0);   /* Configure P2_10, as EINT0 */
    LPC_GPIO2->FIODIR   &= ~(0 << 10);      // P2.10 is an input pin
    LPC_GPIOINT->IO2IntEnF |=  (1 << 10);   // P2.10 reads the falling edges to generate an interrupt

}


void NVIC_SetVectorTable(unsigned long NVIC_VectTab, unsigned long Offset)
{
    /**
     * https://community.arm.com/developer/tools-software/tools/f/keil-forum/39482/how-to-relocate-the-cortex-m3-vector-table
     */
    SCB->VTOR = NVIC_VectTab | (Offset & (unsigned int)0x1FFFFF80);
}



constexpr uint32_t VECTORTABLE_SIZE = 256;

/**
 * @brief Aligment is in bytes!
 *  Global static vector table variable
 */
alignas(VECTORTABLE_SIZE * sizeof(uint32_t)) static std::array<uint32_t, VECTORTABLE_SIZE> g_VectorTable;

void InitRAMInterruptVectorTable()
{

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



void PushButton_Handler()
{
    ClearInterruptFlagEINTX(0);
    ToggleLED(); 
}

void InitEINT0()
{
    LPC_SC->EXTINT      = (1<<SBIT_EINT0);	    /* Clear Pending interrupts */
    LPC_SC->EXTMODE     = (1<<SBIT_EXTMODE0);   /* Configure EINTx as Edge Triggered*/
    LPC_SC->EXTPOLAR    = (1<<SBIT_EXTPOLAR0);  /* Configure EINTx as Falling Edge */
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

void FireInterrupt(uint32_t index)
{
    if (index < 0 || index > 111)
        return;
    
    /**
     * Bits 8:0 - 256 Interrupt values
     * Values of bits 31:9 are reserved, so should not be read or touched
     * Manual UM10360 - Page 92
     */
    NVIC->STIR |= (0xFF & index);
}


int main()
{   
    SystemInit();

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
        vectorTable.RegisterInterruptCallback(18, PushButton_Handler);

        // allow the EINT0 inrettupt to be triggered
        vectorTable.EnableInterrupt(18);


        while(1)
        {
            // trigger interrupt via software (easier testing, less configuration of buttons etc.)
            vectorTable.FireSoftwareInterrupt(18);
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


int main(void)
{
    // make the LED pin an output for PORTB5
    DDRB = 1 << DDB5;       // DDB5 = 5

    while (1)
    {
        _delay_ms(200);

        // toggle the LED
        PORTB ^= 1 << PB5;  // PB5 = 5
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