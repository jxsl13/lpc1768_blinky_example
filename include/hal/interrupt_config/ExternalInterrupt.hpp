#pragma once
#include <config.hpp>


class ExternalInterrupt
{

public:

    /**
     * @brief Defines valid Triggers, like Level Low, 
     * Level High, Falling Edge, Rising Edge, Logical Change, etc.
     */
    enum class TriggerType : ValueType;

    /**
     * @brief List of valid indices that can be used. 
     * We want to reduce the number of checks at runtime.
     */
    enum class IndexType : ValueType;

    /**
     * @brief Construct a new External Interrupt object
     * 
     * @param InterruptIndex EXTI[X]/INT[X]/EINT[X] -> X is the number, e.g. INT0 -> 0
     * @param Trigger 
     */
    ExternalInterrupt(IndexType InterruptIndex, TriggerType Trigger)
    {
        m_Index = InterruptIndex;
        m_Trigger = static_cast<ValueType>(Trigger); // in order to do less conversions late ron.
    };

    ~ExternalInterrupt();


    /**
     * @brief Apply the current configuration to a different External Interrupt.
     *        This does explicitly not change the internally used InterruptIndex, 
     *        it just applies the configuration to the passed InterruptIndex
     * 
     * @param InterruptIndex Valid index that exists for the specific plattform.
     */
    void applyTo(IndexType InterruptIndex) const;

    /**
     * @brief Apply the configuration to the at contruction passed 
     */
    void apply() const
    {
        applyTo(m_Index);
    };

    /**
     * @brief Retrieve configuration from a given external interrupt.
     *          The internal state is updated accordingly to hold that configuration.
     * @param InterruptIndex Index of the External Interrupt(see above).
     */
    void retrieveFrom(IndexType InterruptIndex);

    /**
     * @brief Retrieve the configuration of the currently internally saved Interrupt Index.
     *          The Index is not being changed, the trigger state might change.
     */
    void retrieve()
    {
        retrieveFrom(m_Index);
    };

    private:
        /**
         * These are kept as ValueTypes in order to  
         * better work with them later on.
         */
        IndexType  m_Index;
        ValueType         m_Trigger;
};

