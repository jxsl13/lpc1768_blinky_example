#include "Interrupt.hpp"
#include <string.h>
#include <utils/BitMacros.hpp>
#include <utils/RegisterBits.hpp>


const static RegisterBits<1> s_InterruptEnableBitMap[VectorsCount] = {
    {0x0, 0}, //
    {&EIMSK, INT0},
    {&EIMSK, INT1},
    {&PCICR, PCIE0},
    {&PCICR, PCIE1},
    {&PCICR, PCIE2},
    {&WDTCSR, WDIE},
    {&TIMSK2, OCIE2A},
    {&TIMSK2, OCIE2B},
    {&TIMSK2, TOIE2},
    {&TIMSK1, ICIE1},
    {&TIMSK1, OCIE1A},
    {&TIMSK1, OCIE1B},
    {&TIMSK1, TOIE1},
    {&TIMSK0, OCIE0A},
    {&TIMSK0, OCIE0B},
    {&TIMSK0, TOIE0},
    {&SPCR, SPIE},
    {&UCSR0B, RXCIE0},
    {&UCSR0B, UDRIE0},
    {&UCSR0B, TXCIE0},
    {&ADCSRA, ADIE},
    {&EECR, EERIE},
    {&ACSR, ACIE},
    {&TWCR, TWIE},
    {&SPMCSR, SPMIE},
};

/**
 * @brief Global Interrupt Vector Jump Table that contains function pointers
 * which are executed in each corresponding Interrupt Service Routine.
 * This Table needs to be global in order for it to be accessible from
 * within the ISRs.
 */
static void (*s_VectorTable[VectorsCount])(void);

#define INT0_vect _VECTOR(1)          /* External Interrupt Request 0 */
#define INT1_vect _VECTOR(2)          /* External Interrupt Request 1 */
#define PCINT0_vect _VECTOR(3)        /* Pin Change Interrupt Request 0 */
#define PCINT1_vect _VECTOR(4)        /* Pin Change Interrupt Request 0 */
#define PCINT2_vect _VECTOR(5)        /* Pin Change Interrupt Request 1 */
#define WDT_vect _VECTOR(6)           /* Watchdog Time-out Interrupt */
#define TIMER2_COMPA_vect _VECTOR(7)  /* Timer/Counter2 Compare Match A */
#define TIMER2_COMPB_vect _VECTOR(8)  /* Timer/Counter2 Compare Match A */
#define TIMER2_OVF_vect _VECTOR(9)    /* Timer/Counter2 Compare Match B */
#define TIMER1_CAPT_vect _VECTOR(10)  /* Timer/Counter1 Capture Event */
#define TIMER1_COMPA_vect _VECTOR(11) /* Timer/Counter1 Compare Match A */
#define TIMER1_COMPB_vect _VECTOR(12) /* Timer/Counter1 Compare Match B */
#define TIMER1_OVF_vect _VECTOR(13)   /* Timer/Counter1 Overflow */
#define TIMER0_COMPA_vect _VECTOR(14) /* TimerCounter0 Compare Match A */
#define TIMER0_COMPB_vect _VECTOR(15) /* TimerCounter0 Compare Match B */
#define TIMER0_OVF_vect _VECTOR(16)   /* Timer/Couner0 Overflow */
#define SPI_STC_vect _VECTOR(17)      /* SPI Serial Transfer Complete */
#define USART_RXC_vect _VECTOR(18)    /* USART Rx Complete */
#define USART_UDRE_vect _VECTOR(19)   /* USART, Data Register Empty */
#define USART_TXC_vect _VECTOR(20)    /* USART Tx Complete */
#define ADC_vect _VECTOR(21)          /* ADC Conversion Complete */
#define EE_READY_vect _VECTOR(22)     /* EEPROM Ready */
#define ANALOG_COMP_vect _VECTOR(23)  /* Analog Comparator */
#define TWI_vect _VECTOR(24)          /* Two-wire Serial Interface */
#define SPM_READY_vect _VECTOR(25)    /* Store Program Memory Read */

ISR(INT0_vect)
{
    s_VectorTable[1]();
}
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
ISR(USART_RXC_vect)
{
    s_VectorTable[18]();
}
ISR(USART_UDRE_vect)
{
    s_VectorTable[19]();
}
ISR(USART_TXC_vect)
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

    // set member variable to point at the static array.
    m_VectorTable = reinterpret_cast<ValueType*>(s_VectorTable);
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

bool InterruptVectorTable::isEnabled()
{
    return IS_SET(SREG, 7);
}

void InterruptVectorTable::setCallback(IRQType InterruptIndex, void (*Callback)(void))
{
    /**
     * All needed checks are done at compile time, especially whether 
     * InterruptIndex is actually a valid Interrupt index
     */
    s_VectorTable[static_cast<ValueType>(InterruptIndex)] = Callback;
}

void InterruptVectorTable::enableISR(IRQType InterruptIndex)
{
    ValueType index = static_cast<ValueType>(InterruptIndex);

    volatile ValueType* pRegister = s_InterruptEnableBitMap[index].m_Register;
    ValueType Bit = s_InterruptEnableBitMap[index].m_Bits[0];

    ENABLE(*pRegister, Bit);
}

void InterruptVectorTable::disableISR(IRQType InterruptIndex)
{
    ValueType index = static_cast<ValueType>(InterruptIndex);

    volatile ValueType* pRegister = s_InterruptEnableBitMap[index].m_Register;
    ValueType Bit = s_InterruptEnableBitMap[index].m_Bits[0];

    DISABLE(*pRegister, Bit);
}

bool InterruptVectorTable::isEnabled(IRQType InterruptIndex)
{
    ValueType index = static_cast<ValueType>(InterruptIndex);
    
    volatile ValueType* pRegister = s_InterruptEnableBitMap[index].m_Register;
    ValueType Bit = s_InterruptEnableBitMap[index].m_Bits[0];

    return IS_SET(*pRegister, Bit);
}


void InterruptVectorTable::triggerIRQ(IRQType InterruptIndex)
{

    ValueType index = static_cast<ValueType>(InterruptIndex);

    // check if interrupt is globally enabled as well as the specific ISR
    volatile ValueType* pRegister = s_InterruptEnableBitMap[index].m_Register;
    ValueType Bit = s_InterruptEnableBitMap[index].m_Bits[0];

    if(!IS_SET(SREG, 7) || !IS_SET(*pRegister, Bit))
        return;

    // call the function 
    s_VectorTable[index](); 
}
