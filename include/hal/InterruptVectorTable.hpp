#pragma once

#include <config.hpp>

#define DISABLE(REGISTER, BIT) (REGISTER) &= ~(1 << BIT)
#define ENABLE(REGISTER, BIT) (REGISTER) |= (1 << BIT)
#define TOGGLE(REGISTER, BIT) (REGISTER) ^= (1 << BIT)
#define TRIGGER(REGISTER, BIT) TOGGLE(REGISTER, BIT);TOGGLE(REGISTER, BIT)
#define IS_SET(REGISTER, BIT) (REGISTER & (1 << BIT)) > 0

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
         * @brief Destroy the Interrupt Vector Table object
         */
        ~InterruptVectorTable();

        /**
         * @brief This vector pointer needs to point to the Vector Table.
         * The vector Table needs to have the explicit size of VectorsCount of the ValueType.
         * The behaviour is undefined if the vector is accessed from with offsets that are not within the
         * range [0:VectorsCount]
         */
        ValueType* m_VectorTable;

    public:
        enum IRQTypes : ValueType; // forward declaration of enums with type/size specification.

        /**
         * @brief Deleted copy constructor
         */
        InterruptVectorTable(InterruptVectorTable const&)   = delete;

        /**
         * @brief Deleted asignment operator
         */
        void operator=(InterruptVectorTable const&)         = delete;

        /**
         * @brief Get the Singleton instance
         * Aftr this instance has ben created, interrupts are enabled by default.
         * 
         * @return InterruptVectorTable& 
         */
        static InterruptVectorTable& getInstance()
        {
            static InterruptVectorTable instance;   // Guaranteed to be destroyed.
                                                    // Instantiated on first use.
            return instance;
        }

        /**
         * @brief 
         * 
         * @param InterruptNumber 
         * @param Callback Function to be called, when the Interrupt is 
         */
        bool addCallback(ValueType InterruptNumber, void (*Callback)(void));

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

