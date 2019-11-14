#pragma once

#include <config.hpp>
#include "ValidIRQTypes.hpp"

#define DISABLE(REGISTER, BIT) (REGISTER) &= ~(1 << (BIT))
#define ENABLE(REGISTER, BIT) (REGISTER) |= (1 << (BIT))
#define TOGGLE(REGISTER, BIT) (REGISTER) ^= (1 << (BIT))
#define TRIGGER(REGISTER, BIT) \
    TOGGLE(REGISTER, BIT);     \
    TOGGLE(REGISTER, BIT)
#define IS_SET(REGISTER, BIT) (REGISTER & (1 << (BIT))) != 0


/**
 * @brief If the available interrupt indices do not construct a continuous range, 
 * one could force checks by wrapping code segments into this macro.
 */
#ifndef IRQ_VALIDITY_CHECK
    #define IRQ_INDEX_CHECK(X) X
#else
     #define IRQ_INDEX_CHECK(X) if(IsValidIRQType(InterruptIndex)) {X;}
#endif

/**
 * @brief Contains every available ValueType/Index sorted by value!
 */

class InterruptVectorTable
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
    InterruptVectorTable();


    /**
         * @brief This vector pointer needs to point to the Vector Table.
         * The vector Table needs to have the explicit size of VectorsCount of the ValueType.
         * The behaviour is undefined if the vector is accessed from with offsets that are not within the
         * range [0:VectorsCount]
         */
    ValueType* m_VectorTable;

    /**
      * @brief Checks whether the given IRQNumber is valid.
      * 
      * @param InterruptNumber 
      * @return true Is valid
      * @return false Is not valid
      */

   public:
    enum IRQTypes : ValueType; // forward declaration of enums with type/size specification.
    const static ValidIRQTypes s_ValidIRQTypes;

    bool IsValidIRQType(const ValueType IRQNumber) { return s_ValidIRQTypes.IsValidIRQType(IRQNumber); };

    /**
         * @brief Deleted copy constructor
         */
    InterruptVectorTable(InterruptVectorTable const&) = delete;

    /**
         * @brief Deleted asignment operator
         */
    void operator=(InterruptVectorTable const&) = delete;

    /**
         * @brief Get the Singleton instance
         * Aftr this instance has ben created, interrupts are enabled by default.
         * 
         * @return InterruptVectorTable& 
         */
    static InterruptVectorTable& getInstance()
    {
        static InterruptVectorTable instance; // Guaranteed to be destroyed.
                                              // Instantiated on first use.
        return instance;
    }

    /**
         * @brief 
         * 
         * @param InterruptNumber 
         * @param Callback Function to be called, when the Interrupt is 
         */
    bool setCallback(ValueType InterruptNumber, void (*Callback)(void));

    /**
         * @brief Enables interrupts globally
         */
    void enableIRQ();

    /**
         * @brief Disables Interrupts globally.
         */
    void disableIRQ();

    /**
         * @brief Disable interrupt at given InterruptNumber
         * 
         * @param InterruptNumber interrupt InterruptNumber, as defined in the manuals: >= 0
         */
    void enableISR(ValueType InterruptNumber);

    /**
         * @brief Disable an interrupt / interrupt service routine
         * 
         * @param InterruptNumber Is the InterruptNumber of the interrupt within the interrupt vctor table. 
         *              Should be a ValueType greater than or equal to 0.
         */
    void disableISR(ValueType InterruptNumber);

    /**
         * @brief Triggers a software interrupt request
         * 
         * @param InterruptNumber 
         */
    void triggerIRQ(ValueType InterruptNumber);
};
