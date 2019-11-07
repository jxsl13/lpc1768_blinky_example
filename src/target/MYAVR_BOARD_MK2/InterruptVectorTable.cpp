
#include <string.h>
#include <hal/InterruptVectorTable.hpp>

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

enum InterruptVectorTable::IRQTypes : ValueType
{
    RESET_IDX = 0, // Not available.
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
    USART_RXC_IDX,
    USART_UDRE_IDX,
    USART_TXC_IDX,
    ADC_IDX,
    EE_READY_IDX,
    ANALOG_COMP_IDX,
    TWI_IDX,
    SPM_READY_IDX = 25,
};

enum IRQTrigger : ValueType
{
    LOW_LEVEL       = 0,
    LOGICAL_CHANGE  = 1,
    FALLING_EDGE    = 2,
    RISING_EDGE     = 3,
    TRIGGER_NONE    = 4,
};

/**
 * @brief Global Interrupt Vector Jump Table that contains function pointers
 * which are executed in each corresponding Interrupt Service Routine.
 * This Table needs to be global in order for it to be accessible from
 * within the ISRs.
 */
static void (*s_VectorTable[VectorsCount])(void);

template <ValueType Bits> struct __RegisterBits;

template <ValueType Bits>
struct __RegisterBits
{
    /**
     * @brief Register that is accessed at given bits
     */
    volatile ValueType* m_Register;

    /**
     * @brief Bit positions that configure the register
     *        Negative values indicate the absence of a 
     *        specific bit and all the following bits.
     *        [0,1,5,-1,-1]
     */
    int8_t m_Bits[Bits];

    ValueType getValue()
    {
        if (!m_Register)
            return 0;

        ValueType tmp = 0;

        for (ValueType i = 0; i < Bits; i++)
        {
            if (m_Bits[i] < 0)
                break;
            tmp += *m_Register & (1 << m_Bits[i]);
        }

        return tmp;
    }

    /**
     * @brief Get the Normalized Value object
     * 
     * @return ValueType the value with all bits 
     *          omitted and that are not part of the mask, meaning
     *          every bit that is not part of the mask and is smaller than
     *          the smallest bit position is omitted and the whole value is 
     *          shifted to the right in order to have the smallest used bit index
     *          be on bit index 0. (rshift whole value (smallest bit index -1) positions to the right.) 
     */
    ValueType getRightShiftedValue()
    {
        if (!m_Register)
            return 0;

        ValueType tmpValue = 0;
        int8_t tmpBit = 0;
        ValueType bitsToShift = 8;

        // find smallest bit index
        for (ValueType i = 0; i < Bits; i++)
        {
            tmpBit = m_Bits[i];

            if (tmpBit >= 0 && tmpBit < bitsToShift)
            {
                bitsToShift = tmpBit;
            }    
        }

        if (bitsToShift >= 8)
            bitsToShift = 0;
        
        
        for (ValueType i = 0; i < Bits; i++)
        {   
            tmpBit = m_Bits[i];

            if (tmpBit < 0)
                break;

            tmpValue += *m_Register & (1 << (tmpBit - bitsToShift));

        }
        return tmpValue;    
    }

    int8_t getFirstEnabledBit()
    {
        ValueType smallestIndex = 8;
        int8_t tmpBit = 0;
        for (int8_t i = 0; i < Bits; i++)
        {
            tmpBit = m_Bits[i];
            if(tmpBit >= 0 && IS_SET(*m_Register, tmpBit) && tmpBit < smallestIndex)
            {
                smallestIndex = tmpBit; 
            }   
        }

        if (smallestIndex >= 8)
            return -1;
        else
            return smallestIndex;   
    }

    IRQTrigger getIRQTrigger(ValueType InterruptIndex)
    {
        using irqt = InterruptVectorTable::IRQTypes;
        switch ((unsigned long)InterruptIndex)
    {
        case irqt::RESET_IDX:
            return TRIGGER_NONE;
        case irqt::INT0_IDX:
            return getRightShiftedValue();
            
        case irqt::INT1_IDX:
            return getRightShiftedValue();
        case irqt::PCINT0_IDX:
            if (getValue())
               return LOGICAL_CHANGE;
            else
                return TRIGGER_NONE;                        
        case irqt::PCINT1_IDX:
            if (getValue())
               return LOGICAL_CHANGE;
            else
                return TRIGGER_NONE; 
        case irqt::PCINT2_IDX:
            if (getValue())
               return LOGICAL_CHANGE;
            else
                return TRIGGER_NONE; 
        case irqt::WDT_IDX:
            //ENABLE(WDTCSR, WDIE);
            break;
        case irqt::TIMER2_COMPA_IDX:
            //ENABLE(TIMSK2, OCIE2A);
            break;
        case irqt::TIMER2_COMPB_IDX:
            //ENABLE(TIMSK2, OCIE2B);
            break;
        case irqt::TIMER2_OVF_IDX:
            //ENABLE(TIMSK2, TOIE2);
            break;
        case irqt::TIMER1_CAPT_IDX:
            //ENABLE(TIMSK1, ICIE1);
            break;
        case irqt::TIMER1_COMPA_IDX:
            //ENABLE(TIMSK1, OCIE1A);
            break;
        case irqt::TIMER1_COMPB_IDX:
            //ENABLE(TIMSK1, OCIE1B);
            break;
        case irqt::TIMER1_OVF_IDX:
            //ENABLE(TIMSK1, TOIE1);
            break;
        case irqt::TIMER0_COMPA_IDX:
            //ENABLE(TIMSK0, OCIE0A);
            break;
        case irqt::TIMER0_COMPB_IDX:
            //ENABLE(TIMSK0, OCIE0B);
            break;
        case irqt::TIMER0_OVF_IDX:
            //ENABLE(TIMSK0, TOIE0);
            break;
        case irqt::SPI_STC_IDX:
            //ENABLE(SPCR, SPIE);
            break;
        case irqt::USART_RXC_IDX:
            //ENABLE(UCSR0B, RXCIE0);
            break;
        case irqt::USART_UDRE_IDX:
            //ENABLE(UCSR0B, UDRIE0);
            break;
        case irqt::USART_TXC_IDX:
            //ENABLE(UCSR0B, TXCIE0);
            break;
        case irqt::ADC_IDX:
            //ENABLE(ADCSRA, ADIE);
            break;
        case irqt::EE_READY_IDX:
            //ENABLE(EECR, EERIE);
            break;
        case irqt::ANALOG_COMP_IDX:
            //ENABLE(ACSR, ACIE);
            break;
        case irqt::TWI_IDX:
            //ENABLE(TWCR, TWIE);
            break;
        case irqt::SPM_READY_IDX:
            //ENABLE(SPMCSR, SPMIE);
            break;
        default:
            break;
    }
        
    }
};

const __RegisterBits<1> s_InterruptEnableBitMap[VectorsCount] = {
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
    {&SPMCSR, SPMIE}};

const __RegisterBits<8> s_InterruptConfigBits[VectorsCount] = {
    {0x0, -1,},
    {&EICRA, ISC00, ISC01, -1, }, // INT0, [Bit0, Bit1] // 
    {&EICRA, ISC10, ISC11, -1, }, // INT1, [Bit0, Bit1] // 
    {&PCMSK0, PCINT0, PCINT1, PCINT2, PCINT3, PCINT4, PCINT5, PCINT6, PCINT7},
    {&PCMSK1, PCINT8, PCINT9, PCINT10, PCINT11, PCINT12, PCINT13, PCINT14, -1},
    {&PCMSK2, PCINT16, PCINT17, PCINT18, PCINT19, PCINT20, PCINT21, PCINT22, PCINT23},
    {0x0, -1,},
};

// Maps: 0, 1, 2, 3 -> LOW_LEVEL, LOGICAL_CHANGE, FALLING_EDGE, RISING_EDGE
const IRQTrigger __EICRA_NormalizedValueMapMap[] = {LOW_LEVEL, LOGICAL_CHANGE, FALLING_EDGE, RISING_EDGE};
const IRQTrigger __PCMSKX_Map[] = {};

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
    if (InterruptIndex <= 0 || VectorsCount <= InterruptIndex)
        return false;

    // set callback
    s_VectorTable[InterruptIndex] = Callback;
    return true;
}

void InterruptVectorTable::enableISR(ValueType InterruptIndex)
{
    if (InterruptIndex <= 0 || VectorsCount <= InterruptIndex)
        return;

    volatile ValueType* RegisterPtr = s_InterruptEnableBitMap[InterruptIndex].m_Register;
    ValueType Bit = s_InterruptEnableBitMap[InterruptIndex].m_Bits[0];

    ENABLE(*RegisterPtr, Bit);
}

void InterruptVectorTable::disableISR(ValueType InterruptIndex)
{
    if (InterruptIndex <= 0 || VectorsCount <= InterruptIndex)
        return;

    volatile ValueType* RegisterPtr = s_InterruptEnableBitMap[InterruptIndex].m_Register;
    ValueType Bit = s_InterruptEnableBitMap[InterruptIndex].m_Bits[0];

    DISABLE(*RegisterPtr, Bit);
}

void InterruptVectorTable::triggerIRQ(ValueType InterruptIndex)
{
    if (InterruptIndex < 0 || VectorsCount <= InterruptIndex)
        return;

    // call the function: TODO: look for a better solution.
    //s_VectorTable[InterruptIndex]();
    auto getInterruptTrigger = [](ValueType InterruptIndex) -> IRQTrigger {
        return (IRQTrigger)0;
    };
    auto triggerWithoutChangingState = [](volatile ValueType* Register, ValueType Bit, IRQTrigger config) -> void { 
        switch (config)
        {
            case LOW_LEVEL:

                break;
            case LOGICAL_CHANGE:

                break;
            case FALLING_EDGE:

                break;
            case RISING_EDGE:

                break;
            default:
                break;
        }
        ; };

    switch ((unsigned long)InterruptIndex)
    {
        case RESET_IDX:
            break;
        case INT0_IDX:
            triggerWithoutChangingState(&PIND, PIND2, IRQTrigger::LOW_LEVEL);

            break;
        case INT1_IDX:
            //ENABLE(EIMSK, INT1);
            break;
        case PCINT0_IDX:
            //ENABLE(PCICR, PCIE0);
            break;
        case PCINT1_IDX:
            //ENABLE(PCICR, PCIE1);
            break;
        case PCINT2_IDX:
            //ENABLE(PCICR, PCIE2);
            break;
        case WDT_IDX:
            //ENABLE(WDTCSR, WDIE);
            break;
        case TIMER2_COMPA_IDX:
            //ENABLE(TIMSK2, OCIE2A);
            break;
        case TIMER2_COMPB_IDX:
            //ENABLE(TIMSK2, OCIE2B);
            break;
        case TIMER2_OVF_IDX:
            //ENABLE(TIMSK2, TOIE2);
            break;
        case TIMER1_CAPT_IDX:
            //ENABLE(TIMSK1, ICIE1);
            break;
        case TIMER1_COMPA_IDX:
            //ENABLE(TIMSK1, OCIE1A);
            break;
        case TIMER1_COMPB_IDX:
            //ENABLE(TIMSK1, OCIE1B);
            break;
        case TIMER1_OVF_IDX:
            //ENABLE(TIMSK1, TOIE1);
            break;
        case TIMER0_COMPA_IDX:
            //ENABLE(TIMSK0, OCIE0A);
            break;
        case TIMER0_COMPB_IDX:
            //ENABLE(TIMSK0, OCIE0B);
            break;
        case TIMER0_OVF_IDX:
            //ENABLE(TIMSK0, TOIE0);
            break;
        case SPI_STC_IDX:
            //ENABLE(SPCR, SPIE);
            break;
        case USART_RXC_IDX:
            //ENABLE(UCSR0B, RXCIE0);
            break;
        case USART_UDRE_IDX:
            //ENABLE(UCSR0B, UDRIE0);
            break;
        case USART_TXC_IDX:
            //ENABLE(UCSR0B, TXCIE0);
            break;
        case ADC_IDX:
            //ENABLE(ADCSRA, ADIE);
            break;
        case EE_READY_IDX:
            //ENABLE(EECR, EERIE);
            break;
        case ANALOG_COMP_IDX:
            //ENABLE(ACSR, ACIE);
            break;
        case TWI_IDX:
            //ENABLE(TWCR, TWIE);
            break;
        case SPM_READY_IDX:
            //ENABLE(SPMCSR, SPMIE);
            break;
        default:
            break;
    }
}
