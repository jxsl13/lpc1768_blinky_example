#pragma once

#include <utils/BitMacros.hpp>
#include <utils/RegisterBits.hpp>

#include <hal/Interrupt.hpp>
#include "InterruptType.hpp"
#include "device.hpp"



/**
 * @brief Global Interrupt Vector Jump Table that contains function pointers
 * which are executed in each corresponding Interrupt Service Routine.
 * This Table needs to be global in order for it to be accessible from
 * within the ISRs.
 */
extern void (*g_VectorTable[holmes::internal::DeviceAtMega328p::s_NumInterruptVectors])(void);



namespace holmes
{
namespace internal
{


/**
 * @brief Evaluated at compile time, static(accessible only from within this source file), 
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
        g_VectorTable[0] = 0x0;

        // set default handler for all the other ISRs
        for (uint8_t i = 1; i < DeviceAtMega328p::s_NumInterruptVectors; i++)
        {
            g_VectorTable[i] = DefaultHandler;
        }

        // set member variable to point at the static array.
        m_VectorTable = reinterpret_cast<uint8_t*>(g_VectorTable);
    }

    /**
     * @brief This vector pointer needs to point to the Vector Table.
     * The vector Table needs to have the explicit size of DeviceAtMega328p::s_NumInterruptVectors of the uint8_t.
     * The behaviour is undefined if the vector is accessed with offsets that are not within the
     * range [0:DeviceAtMega328p::s_NumInterruptVectors[
     */
    uint8_t* m_VectorTable;

   public:
    
    static auto getInstance() -> InterruptVectorTable&
    {   
        // Info: It is not possible to properly define desructors for rather complex
        // types, in this case a static variable, thus destructors have been omitted.
        static InterruptVectorTable instance; // Guaranteed to be destroyed.
                                              // Instantiated on first use.
        return instance;
    }

    auto setCallback(IRQType InterruptIndex, void (*Callback)(void)) -> void
    {
        /**
         * All needed checks are done at compile time, especially whether 
         * InterruptIndex is actually a valid Interrupt index,
         * as enum class variables are enfoced to be typesafe contrary to simple enums
         */
        g_VectorTable[static_cast<uint8_t>(InterruptIndex)] = Callback;
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
        // check if interrupts are globally enabled (Manual Page 20)
        return IS_SET(SREG, 7);
    }

    auto enableISR(IRQType InterruptIndex) -> void
    {
        uint8_t index = static_cast<uint8_t>(InterruptIndex);

        // get register address & bit that needs to be set from the map structure
        volatile uint8_t* pRegister = s_InterruptEnableBitMap[index].m_Register;
        uint8_t Bit = s_InterruptEnableBitMap[index].m_Bits[0];

        // enable set bit to 1 within that register 
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
        g_VectorTable[index]();
    }

    auto waitForIRQ() -> void
    {
        // SLEEP_MODE_IDLE needs 6 ticks to wake up, but does react to ALL possible interrupts.
        // other sleep modes need longer to wake up and do not react to all interrupts. (Manual Page 48)
        set_sleep_mode(SLEEP_MODE_IDLE);    // set sleep mode
        sei();                              // enable interrupts
        sleep_mode();                       // sleep until interrupt
    }
};


} // namespace internal
} // namespace holmes
