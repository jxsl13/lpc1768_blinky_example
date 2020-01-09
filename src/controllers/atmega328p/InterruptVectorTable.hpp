#pragma once

#include <avr/interrupt.h>
#include <avr/sleep.h>

#include <utils/BitMacros.hpp>
#include <utils/RegisterBits.hpp>

#include <hal/Interrupt.hpp>
#include "device.hpp"


namespace holmes
{
namespace internal
{
/**
 * @brief Contains every available uint8_t/Index sorted by value!
 */
/**
 * @brief Global Interrupt Vector Jump Table that contains function pointers
 * which are executed in each corresponding Interrupt Service Routine.
 * This Table needs to be global in order for it to be accessible from
 * within the ISRs.
 */
static void (*s_VectorTable[DeviceAtMega328p::s_NumInterruptVectors])(void);

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

/**
 * @brief Evaluated at compile time, static(accessible only from within this cpp file), 
 * immutable
 */
constexpr const static RegisterBits<uint8_t, 1> s_InterruptEnableBitMap[DeviceAtMega328p::s_NumInterruptVectors] = {
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

enum class IRQType : uint8_t
    {
        //RESET_IRQn = 0,               // Not available.
        INT0_IRQn = 1,
        INT1_IRQn = 2,
        PCINT0_IRQn = 3,
        PCINT1_IRQn = 4,
        PCINT2_IRQn = 5,
        WDT_IRQn = 6,
        TIMER2_COMPA_IRQn = 7,
        TIMER2_COMPB_IRQn = 8,
        TIMER2_OVF_IRQn = 9,
        TIMER1_CAPT_IRQn = 10,
        TIMER1_COMPA_IRQn = 11,
        TIMER1_COMPB_IRQn = 12,
        TIMER1_OVF_IRQn = 13,
        TIMER0_COMPA_IRQn = 14,
        TIMER0_COMPB_IRQn = 15,
        TIMER0_OVF_IRQn = 16,
        SPI_STC_IRQn = 17,
        USART_RXC_IRQn = 18,
        USART_UDRE_IRQn = 19,
        USART_TXC_IRQn = 20,
        ADC_IRQn = 21,
        EE_READY_IRQn = 22,
        ANALOG_COMP_IRQn = 23,
        TWI_IRQn = 24,
        SPM_READY_IRQn = 25
};


#ifdef __cplusplus
extern "C" {
#endif

template <>
class InterruptVectorTable<DeviceAtMega328p, uint8_t, IRQType>
{
    /**
     * @brief 
     * Singleton design pattern:
     * https://stackoverflow.com/questions/1008019/c-singleton-design-pattern
     */

   private:
    /**
     * @brief Construct a new Interrupt Vector Table object
     */
    InterruptVectorTable()
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
        for (uint8_t i = 1; i < DeviceAtMega328p::s_NumInterruptVectors; i++)
        {
            s_VectorTable[i] = DefaultHandler;
        }

        // set member variable to point at the static array.
        m_VectorTable = reinterpret_cast<uint8_t*>(s_VectorTable);
    }

    /**
     * @brief This vector pointer needs to point to the Vector Table.
     * The vector Table needs to have the explicit size of VectorsCount of the uint8_t.
     * The behaviour is undefined if the vector is accessed from with offsets that are not within the
     * range [0:VectorsCount]
     */
    uint8_t* m_VectorTable;

   public:
    
    static auto getInstance() -> InterruptVectorTable&
    {
        static InterruptVectorTable instance; // Guaranteed to be destroyed.
                                              // Instantiated on first use.
        return instance;
    }

    auto setCallback(IRQType InterruptIndex, void (*Callback)(void)) -> void
    {
        /**
         * All needed checks are done at compile time, especially whether 
         * InterruptIndex is actually a valid Interrupt index
         */
        s_VectorTable[static_cast<uint8_t>(InterruptIndex)] = Callback;
    }

    auto enableIRQ() -> void
    {
        // Set interrupt enabled
        sei();
    }

    auto disableIRQ() -> void
    {
        // clear interrupt enabled
        cli();
    }

    auto isEnabled() -> bool
    {
        return IS_SET(SREG, 7);
    }

    auto enableISR(IRQType InterruptIndex) -> void
    {
        uint8_t index = static_cast<uint8_t>(InterruptIndex);

        volatile uint8_t* pRegister = s_InterruptEnableBitMap[index].m_Register;
        uint8_t Bit = s_InterruptEnableBitMap[index].m_Bits[0];

        ENABLE(*pRegister, Bit);
    }

    auto disableISR(IRQType InterruptIndex) -> void
    {
        uint8_t index = static_cast<uint8_t>(InterruptIndex);

        volatile uint8_t* pRegister = s_InterruptEnableBitMap[index].m_Register;
        uint8_t Bit = s_InterruptEnableBitMap[index].m_Bits[0];

        DISABLE(*pRegister, Bit);
    }

    auto isEnabled(IRQType InterruptIndex) -> bool
    {
        uint8_t index = static_cast<uint8_t>(InterruptIndex);

        volatile uint8_t* pRegister = s_InterruptEnableBitMap[index].m_Register;
        uint8_t Bit = s_InterruptEnableBitMap[index].m_Bits[0];

        return IS_SET(*pRegister, Bit);
    }

    auto triggerIRQ(IRQType InterruptIndex) -> void
    {
        uint8_t index = static_cast<uint8_t>(InterruptIndex);

        // check if interrupt is globally enabled as well as the specific ISR
        volatile uint8_t* pRegister = s_InterruptEnableBitMap[index].m_Register;
        uint8_t Bit = s_InterruptEnableBitMap[index].m_Bits[0];

        // emulate interrupt call
        if (!IS_SET(SREG, 7) || !IS_SET(*pRegister, Bit))
            return;

        // call the function
        s_VectorTable[index]();
    }

    auto waitForIRQ() -> void
    {
        // todo: test these two commands
        //__enable_interrupt(); /* set Global Interrupt Enable */
        //__sleep(); /
        set_sleep_mode(SLEEP_MODE_IDLE);
        sei();
        sleep_mode();
    }
};

#ifdef __cplusplus
}
#endif

} // namespace internal
} // namespace holmes
