#include <avr/interrupt.h>
#include <avr/sleep.h>

#include <utils/BitMacros.hpp>
#include <utils/RegisterBits.hpp>

#include "device.hpp"
#include "InterruptVectorTable.hpp"


/**
 * @brief Global Interrupt Vector Jump Table that contains function pointers
 * which are executed in each corresponding Interrupt Service Routine.
 * This Table needs to be global in order for it to be accessible from
 * within the ISRs.
 * 
 * The RESET IRQ is being ignored, thus only 25 of 26 interrupts are defined.
 */
static void (*s_VectorTable[25])(void) = {0x0,};

/**
 * This is simply a redefinition of the already existing macros, 
 * that are not directly visible, but can be seen in the 
 * avr-libc documentation.
 */
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
    s_VectorTable[0]();
}
ISR(INT1_vect)
{
    s_VectorTable[1]();
}
ISR(PCINT0_vect)
{
    s_VectorTable[2]();
}
ISR(PCINT1_vect)
{
    s_VectorTable[3]();
}
ISR(PCINT2_vect)
{
    s_VectorTable[4]();
}
ISR(WDT_vect)
{
    s_VectorTable[5]();
}
ISR(TIMER2_COMPA_vect)
{
    s_VectorTable[6]();
}
ISR(TIMER2_COMPB_vect)
{
    s_VectorTable[7]();
}
ISR(TIMER2_OVF_vect)
{
    s_VectorTable[8]();
}
ISR(TIMER1_CAPT_vect)
{
    s_VectorTable[9]();
}
ISR(TIMER1_COMPA_vect)
{
    s_VectorTable[10]();
}
ISR(TIMER1_COMPB_vect)
{
    s_VectorTable[11]();
}
ISR(TIMER1_OVF_vect)
{
    s_VectorTable[12]();
}
ISR(TIMER0_COMPA_vect)
{
    s_VectorTable[13]();
}
ISR(TIMER0_COMPB_vect)
{
    s_VectorTable[14]();
}
ISR(TIMER0_OVF_vect)
{
    s_VectorTable[15]();
}
ISR(SPI_STC_vect)
{
    s_VectorTable[16]();
}
ISR(USART_RXC_vect)
{
    s_VectorTable[17]();
}
ISR(USART_UDRE_vect)
{
    s_VectorTable[18]();
}
ISR(USART_TXC_vect)
{
    s_VectorTable[19]();
}
ISR(ADC_vect)
{
    s_VectorTable[20]();
}
ISR(EE_READY_vect)
{
    s_VectorTable[21]();
}
ISR(ANALOG_COMP_vect)
{
    s_VectorTable[22]();
}
ISR(TWI_vect)
{
    s_VectorTable[23]();
}
ISR(SPM_READY_vect)
{
    s_VectorTable[24]();
}



namespace holmes
{
namespace internal
{

    /**
     * @brief Evaluated at compile time, static(accessible only from within this source file), 
     * immutable
     * 
     * The RESET interrupt is ignore,d thus only 25 interrupts are used in total.
     */
    constexpr const static RegisterBits<uint8_t, 1> s_InterruptEnableBitMap[25] = {
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

    
    InterruptVectorTable<DeviceAtMega328p, IRQType>::InterruptVectorTable()
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


        // set default handler for all the ISRs
        for (uint8_t i = 0; i < 25; i++)
        {
            s_VectorTable[i] = DefaultHandler;
        }
    }

    
    auto InterruptVectorTable<DeviceAtMega328p, IRQType>::getInstance() -> InterruptVectorTable&
    {   
        // Info: It is not possible to properly define desructors for rather complex
        // types, in this case a static variable, thus destructors have been omitted.
        static InterruptVectorTable instance; // Guaranteed to be destroyed.
                                              // Instantiated on first use.
        return instance;
    }

    auto InterruptVectorTable<DeviceAtMega328p, IRQType>::setISR(IRQType InterruptIndex, void (*Callback)(void)) -> void
    {
        /**
         * All needed checks are done at compile time, especially whether 
         * InterruptIndex is actually a valid Interrupt index,
         * as enum class variables are enfoced to be typesafe contrary to simple enums
         */
        s_VectorTable[static_cast<uint8_t>(InterruptIndex)] = Callback;
    }

    auto InterruptVectorTable<DeviceAtMega328p, IRQType>::enableIRQ() -> void
    {
        // Set interrupt enabled
        sei();
    }

    auto InterruptVectorTable<DeviceAtMega328p, IRQType>::disableIRQ() -> void
    {
        // clear interrupt enabled
        cli();
    }

    auto InterruptVectorTable<DeviceAtMega328p, IRQType>::isEnabledIRQ() -> bool
    {
        // check if interrupts are globally enabled (Manual Page 20)
        return IS_SET(SREG, 7);
    }

    auto InterruptVectorTable<DeviceAtMega328p, IRQType>::enableISR(IRQType InterruptIndex) -> void
    {
        uint8_t index = static_cast<uint8_t>(InterruptIndex);

        // get register address & bit that needs to be set from the map structure
        volatile uint8_t* pRegister = s_InterruptEnableBitMap[index].m_Register;
        uint8_t Bit = s_InterruptEnableBitMap[index].m_Bits[0];

        // enable set bit to 1 within that register 
        ENABLE(*pRegister, Bit);
    }

    auto InterruptVectorTable<DeviceAtMega328p, IRQType>::disableISR(IRQType InterruptIndex) -> void
    {
        uint8_t index = static_cast<uint8_t>(InterruptIndex);

        volatile uint8_t* pRegister = s_InterruptEnableBitMap[index].m_Register;
        uint8_t Bit = s_InterruptEnableBitMap[index].m_Bits[0];

        DISABLE(*pRegister, Bit);
    }

    auto InterruptVectorTable<DeviceAtMega328p, IRQType>::isEnabledISR(IRQType InterruptIndex) -> bool
    {
        uint8_t index = static_cast<uint8_t>(InterruptIndex);

        volatile uint8_t* pRegister = s_InterruptEnableBitMap[index].m_Register;
        uint8_t Bit = s_InterruptEnableBitMap[index].m_Bits[0];

        return IS_SET(*pRegister, Bit);
    }

    auto InterruptVectorTable<DeviceAtMega328p, IRQType>::triggerISR(IRQType InterruptIndex) -> void
    {
        uint8_t index = static_cast<uint8_t>(InterruptIndex);

        volatile uint8_t* pRegister = s_InterruptEnableBitMap[index].m_Register;
        uint8_t Bit = s_InterruptEnableBitMap[index].m_Bits[0];

        // check if interrupt is globally enabled as well as the specific ISR
        if (!IS_SET(SREG, 7) || !IS_SET(*pRegister, Bit))
            return;

        // emulate interrupt call by calling the function
        s_VectorTable[index]();
    }

    auto InterruptVectorTable<DeviceAtMega328p, IRQType>::waitForIRQ() -> void
    {
        // SLEEP_MODE_IDLE needs 6 ticks to wake up, but does react to ALL possible interrupts.
        // other sleep modes need longer to wake up and do not react to all interrupts. (Manual Page 48)
        set_sleep_mode(SLEEP_MODE_IDLE);    // set sleep mode
        sleep_mode();                       // sleep until interrupt
    }


} // namespace internal
} // namespace holmes

