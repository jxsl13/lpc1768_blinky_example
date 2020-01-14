#pragma once

#include <avr/interrupt.h>
#include <avr/sleep.h>

#include <utils/BitMacros.hpp>
#include <utils/RegisterBits.hpp>

#include <hal/Interrupt.hpp>
#include "device.hpp"



/**
 * @brief Global Interrupt Vector Jump Table that contains function pointers
 * which are executed in each corresponding Interrupt Service Routine.
 * This Table needs to be global in order for it to be accessible from
 * within the ISRs.
 */
extern void (*s_VectorTable[holmes::internal::DeviceAtMega328p::s_NumInterruptVectors])(void);



namespace holmes
{
namespace internal
{
/**
 * @brief Contains every available uint8_t/Index sorted by value!
 */



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

/**
 * @brief List of possible Interrupts to be configured.
 */
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


/**
 * @brief Specialization of the InterruptVectorTable for the DeviceAtMega328p
 * 
 * @tparam  
 */
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

        volatile uint8_t* pRegister = s_InterruptEnableBitMap[index].m_Register;
        uint8_t Bit = s_InterruptEnableBitMap[index].m_Bits[0];

        // check if interrupt is globally enabled as well as the specific ISR
        if (!IS_SET(SREG, 7) || !IS_SET(*pRegister, Bit))
            return;

        // emulate interrupt call by calling the function
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


} // namespace internal
} // namespace holmes
