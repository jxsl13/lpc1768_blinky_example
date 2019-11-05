
#include <hal/InterruptVectorTable.hpp>
#include <string.h>

#define INT0_vect         _VECTOR(1)  /* External Interrupt Request 0 */
#define INT1_vect         _VECTOR(2)  /* External Interrupt Request 1 */
#define PCINT0_vect       _VECTOR(3)  /* Pin Change Interrupt Request 0 */
#define PCINT1_vect       _VECTOR(4)  /* Pin Change Interrupt Request 0 */
#define PCINT2_vect       _VECTOR(5)  /* Pin Change Interrupt Request 1 */
#define WDT_vect          _VECTOR(6)  /* Watchdog Time-out Interrupt */
#define TIMER2_COMPA_vect _VECTOR(7)  /* Timer/Counter2 Compare Match A */
#define TIMER2_COMPB_vect _VECTOR(8)  /* Timer/Counter2 Compare Match A */
#define TIMER2_OVF_vect   _VECTOR(9)  /* Timer/Counter2 Compare Match B */
#define TIMER1_CAPT_vect  _VECTOR(10)  /* Timer/Counter1 Capture Event */
#define TIMER1_COMPA_vect _VECTOR(11)  /* Timer/Counter1 Compare Match A */
#define TIMER1_COMPB_vect _VECTOR(12)  /* Timer/Counter1 Compare Match B */
#define TIMER1_OVF_vect   _VECTOR(13)  /* Timer/Counter1 Overflow */
#define TIMER0_COMPA_vect _VECTOR(14)  /* TimerCounter0 Compare Match A */
#define TIMER0_COMPB_vect _VECTOR(15)  /* TimerCounter0 Compare Match B */
#define TIMER0_OVF_vect   _VECTOR(16)  /* Timer/Couner0 Overflow */
#define SPI_STC_vect      _VECTOR(17)  /* SPI Serial Transfer Complete */
#define USART_RX_vect     _VECTOR(18)  /* USART Rx Complete */
#define USART_UDRE_vect   _VECTOR(19)  /* USART, Data Register Empty */
#define USART_TX_vect     _VECTOR(20)   /* USART Tx Complete */
#define ADC_vect          _VECTOR(21)   /* ADC Conversion Complete */
#define EE_READY_vect     _VECTOR(22)   /* EEPROM Ready */
#define ANALOG_COMP_vect  _VECTOR(23)  /* Analog Comparator */
#define TWI_vect          _VECTOR(24)  /* Two-wire Serial Interface */
#define SPM_READY_vect    _VECTOR(25)  /* Store Program Memory Read */




enum InterruptVectorTable::InterruptTypes : ValueType {
    INT0_IDX = 1,
    INT1_IDX,
    PCINT0_IDX,
    PCINT1_IDX,
    PCINT2_IDX,
    WDT_IDX,
    TIMER2_COMPA_IDX,
    TIMER2_COMPB_IDX,
    TIMER2_OVF_IDX,
    TIMER1_CAPT_IDX,
    TIMER1_COMPA_IDX,
    TIMER1_COMPB_IDX,
    TIMER1_OVF_IDX,
    TIMER0_COMPA_IDX,
    TIMER0_COMPB_IDX,
    TIMER0_OVF_IDX,
    SPI_STC_IDX,
    USART_RX_IDX,
    USART_UDRE_IDX,
    USART_TX_IDX,
    ADC_IDX,
    EE_READY_IDX,
    ANALOG_COMP_IDX,
    TWI_IDX,
    SPM_READY_IDX,
};

/**
 * @brief Global Interrupt Vector Jump Table that contains function pointers
 * which are executed in each corresponding Interrupt Service Routine.
 * This Table needs to be global in order for it to be accessible from
 * within the ISRs.
 */
static void(*s_VectorTable[VectorsCount])(void);



ISR(INT1_vect)
{
    s_VectorTable[2]();
}
ISR(PCINT0_vect)
{
    s_VectorTable[3]();
}
ISR(PCINT1_vect)
{
    s_VectorTable[4]();
}
ISR(PCINT2_vect)
{
    s_VectorTable[5]();
}
ISR(WDT_vect)
{
    s_VectorTable[6]();
}
ISR(TIMER2_COMPA_vect)
{
    s_VectorTable[7]();
}
ISR(TIMER2_COMPB_vect)
{
    s_VectorTable[8]();
}
ISR(TIMER2_OVF_vect)
{
    s_VectorTable[9]();
}
ISR(TIMER1_CAPT_vect)
{
    s_VectorTable[10]();
}
ISR(TIMER1_COMPA_vect)
{
    s_VectorTable[11]();
}
ISR(TIMER1_COMPB_vect)
{
    s_VectorTable[12]();
}
ISR(TIMER1_OVF_vect)
{
    s_VectorTable[13]();
}
ISR(TIMER0_COMPA_vect)
{
    s_VectorTable[14]();
}
ISR(TIMER0_COMPB_vect)
{
    s_VectorTable[15]();
}
ISR(TIMER0_OVF_vect)
{
    s_VectorTable[16]();
}
ISR(SPI_STC_vect)
{
    s_VectorTable[17]();
}
ISR(USART_RX_vect)
{
    s_VectorTable[18]();
}
ISR(USART_UDRE_vect)
{
    s_VectorTable[19]();
}
ISR(USART_TX_vect)
{
    s_VectorTable[20]();
}
ISR(ADC_vect)
{
    s_VectorTable[21]();
}
ISR(EE_READY_vect)
{
    s_VectorTable[22]();
}
ISR(ANALOG_COMP_vect)
{
    s_VectorTable[23]();
}
ISR(TWI_vect)
{
    s_VectorTable[24]();
}
ISR(SPM_READY_vect)
{
    s_VectorTable[25]();
}

InterruptVectorTable::InterruptVectorTable()
{   
    /**
     * Disable interrupts globally, 
     * as we need to firstly configure 
     * the callbacks before enabling the interrupts.
     */
    disableIRQ();

    /**
     * When there is no handler, we use this anonymous DafaultHandler
     */
    auto DefaultHandler = []() -> void {};

    // RESET cannot be set via the interrupt vector table
    s_VectorTable[0] = 0x0;

    // set default handler for all the other ISRs
    for (ValueType i = 1; i < VectorsCount; i++)
    {
        s_VectorTable[i] = DefaultHandler;
    }

    // set memver variable to point at the static array.
    m_VectorTable = reinterpret_cast<ValueType*>(s_VectorTable);
}

InterruptVectorTable::~InterruptVectorTable()
{
}

void InterruptVectorTable::enableIRQ()
{
    // Set interrupt enabled
    sei();
}

void InterruptVectorTable::disableIRQ()
{
    // clear interrupt enabled
    cli();
}

bool InterruptVectorTable::addCallback(ValueType InterruptIndex, void (*Callback)(void))
{   
    // setting index 0 -> not supported
    if (0 <= InterruptIndex || InterruptIndex >= VectorsCount)
        return false;
    
    // set callback
    s_VectorTable[InterruptIndex] = Callback;
    return true;
}

void InterruptVectorTable::enableISR(ValueType InterruptIndex)
{
    switch (InterruptIndex)
    {
        case INT0_IDX:
            EIMSK |= (1 << INT0);
            break;
        case INT1_IDX:
            EIMSK |= (1 << INT1);
            break;
        case PCINT0_IDX:
            PCICR |= (1 << PCIE0);
            break;
        case PCINT1_IDX:
            PCICR |= (1 << PCIE1);
            break;
        case PCINT2_IDX:
            PCICR |= (1 << PCIE2);
            break;
        case WDT_IDX:

            break;
        case TIMER2_COMPA_IDX:

            break;
        case TIMER2_COMPB_IDX:

            break;
        case TIMER2_OVF_IDX:

            break;
        case TIMER1_CAPT_IDX:

            break;
        case TIMER1_COMPA_IDX:

            break;
        case TIMER1_COMPB_IDX:

            break;
        case TIMER1_OVF_IDX:

            break;
        case TIMER0_COMPA_IDX:

            break;
        case TIMER0_COMPB_IDX:

            break;
        case TIMER0_OVF_IDX:

            break;
        case SPI_STC_IDX:

            break;
        case USART_RX_IDX:

            break;
        case USART_UDRE_IDX:

            break;
        case USART_TX_IDX:

            break;
        case ADC_IDX:

            break;
        case EE_READY_IDX:

            break;
        case ANALOG_COMP_IDX:

            break;
        case TWI_IDX:

            break;
        case SPM_READY_IDX:

            break;
        default:
            break;
    }
}

void InterruptVectorTable::disableISR(ValueType InterruptIndex)
{
    switch (InterruptIndex)
    {
        case INT0_IDX:
            EIMSK &= ~(1 << INT0);
            break;
        case INT1_IDX:
            EIMSK &= ~(1 << INT1);
            break;
        case PCINT0_IDX:
            // Page 82
            PCICR |= (1 << PCIE0);
            break;
        case PCINT1_IDX:
            // Page 82
            PCICR |= (1 << PCIE1);
            break;
        case PCINT2_IDX:
            // Page 82
            PCICR |= (1 << PCIE2);
            break;
        case WDT_IDX:

            break;
        case TIMER2_COMPA_IDX:

            break;
        case TIMER2_COMPB_IDX:

            break;
        case TIMER2_OVF_IDX:

            break;
        case TIMER1_CAPT_IDX:

            break;
        case TIMER1_COMPA_IDX:

            break;
        case TIMER1_COMPB_IDX:

            break;
        case TIMER1_OVF_IDX:

            break;
        case TIMER0_COMPA_IDX:

            break;
        case TIMER0_COMPB_IDX:

            break;
        case TIMER0_OVF_IDX:

            break;
        case SPI_STC_IDX:

            break;
        case USART_RX_IDX:

            break;
        case USART_UDRE_IDX:

            break;
        case USART_TX_IDX:

            break;
        case ADC_IDX:

            break;
        case EE_READY_IDX:

            break;
        case ANALOG_COMP_IDX:

            break;
        case TWI_IDX:

            break;
        case SPM_READY_IDX:

            break;
        default:
            break;
    }
}

void InterruptVectorTable::triggerIRQ(ValueType InterruptIndex)
{
    auto triggerInterrupt = [](uint8_t& Register)
    {
        uint8_t tmp = Register;
        Register |= 0xFF;
        Register &= 0x00;
        Register ^= tmp;
    };

    

    // TODO: check if this triggers an interrupt
    switch (InterruptIndex)
    {
        case INT0_IDX:
            EIFR |= (1 << INTF0);
            break;
        case INT1_IDX:
            EIFR |= (1 << INTF1);
            break;
        case PCINT0_IDX:

            break;
        case PCINT1_IDX:

            break;
        case PCINT2_IDX:

            break;
        case WDT_IDX:

            break;
        case TIMER2_COMPA_IDX:

            break;
        case TIMER2_COMPB_IDX:

            break;
        case TIMER2_OVF_IDX:

            break;
        case TIMER1_CAPT_IDX:

            break;
        case TIMER1_COMPA_IDX:

            break;
        case TIMER1_COMPB_IDX:

            break;
        case TIMER1_OVF_IDX:

            break;
        case TIMER0_COMPA_IDX:

            break;
        case TIMER0_COMPB_IDX:

            break;
        case TIMER0_OVF_IDX:

            break;
        case SPI_STC_IDX:

            break;
        case USART_RX_IDX:

            break;
        case USART_UDRE_IDX:

            break;
        case USART_TX_IDX:

            break;
        case ADC_IDX:

            break;
        case EE_READY_IDX:

            break;
        case ANALOG_COMP_IDX:

            break;
        case TWI_IDX:

            break;
        case SPM_READY_IDX:

            break;
        default:
            break;
    }   
}
