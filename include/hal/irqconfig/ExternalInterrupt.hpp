#pragma once

namespace holmes
{
namespace internal
{

    template<class Device, class ConfigType, class TriggerType>
    class ExternalInterrupt
    {
    public:

        /**
         * @brief Construct a new External Interrupt object
         * 
         * @param ConfigIndex EXTI[X]/INT[X]/EINT[X] -> X is the number, e.g. INT0 -> 0
         *                      Some controllers need extra configuration, for example, which Pin triggers the interrupt.
         *                      This index encodes pin wiring as well as which external interrupt index is configured.
         * @param Trigger       What triggers the interrupt.
         */
        ExternalInterrupt(ConfigType ConfigIndex, TriggerType Trigger);

        /**
         * @brief Apply the current configuration to a different External Interrupt.
         *        This does explicitly not change the internally used ConfigIndex, 
         *        it just applies the configuration to the passed ConfigIndex
         * 
         * @param ConfigIndex Valid index that exists for the specific plattform.
         */
        auto applyTo(ConfigType ConfigIndex) const -> void;

        /**
         * @brief Apply the configuration to the at contruction passed 
         */
        auto apply() const -> void;

        /**
         * @brief Clear the pending bit of the given external interrupt index.
         * @param ConfigIndex 
         */
        static auto clearPendingBitOf(ConfigType ConfigIndex) -> void;

        /**
         * @brief Interrupts cause specific bits to be set in a register, 
         * which cause either the interrupt to trigger again, or the other way around,
         * they caus ethe interrupt to trigger continuously, if the bit is not cleared.
         * Both cases should be cleared with this member function.
         */
        auto clearPendingBit() -> void;
    };

}
}