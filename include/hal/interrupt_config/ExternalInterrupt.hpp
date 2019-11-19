#pragma once


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
     * @param ConfigMask    Some controllers need extra configuration, for example, which Pin triggers the interrupt.
     *                      This Mask can be used to set specific Pins/Ports etc. Set specific bits to configure the additional
     *                      needed configuration.
     */
    ExternalInterrupt(IndexType InterruptIndex, TriggerType Trigger)
    {
        m_Index = InterruptIndex;
        m_Trigger = static_cast<ValueType>(Trigger); // in order to do less conversions later on.
    };


    auto getTrigger() const -> TriggerType 
    { 
        return static_cast<TriggerType>(m_Trigger); 
    }


    /**
     * @brief Apply the current configuration to a different External Interrupt.
     *        This does explicitly not change the internally used InterruptIndex, 
     *        it just applies the configuration to the passed InterruptIndex
     * 
     * @param InterruptIndex Valid index that exists for the specific plattform.
     */
    auto applyTo(IndexType InterruptIndex) const -> void;

    /**
     * @brief Apply the configuration to the at contruction passed 
     */
    auto apply() const -> void
    {
        applyTo(m_Index);
    };

    /**
     * @brief Clear the pending bit of the given external interrupt index.
     * @param InterruptIndex 
     */
    auto clearPendingBitOf(ExternalInterrupt::IndexType InterruptIndex) -> void;

    /**
     * @brief Interrupts cause specific bits to be set in a register, 
     * which cause either the interrupt to trigger again, or the other way around,
     * they caus ethe interrupt to trigger continuously, if the bit is not cleared.
     * Both cases should be cleared with this member function.
     */
    auto clearPendingBit() -> void
    {
        clearPendingBitOf(m_Index);
    };

    private:
        /**
         * These are kept as ValueTypes in order to  
         * better work with them later on.
         */
        IndexType  m_Index;
        ValueType  m_Trigger;
};

