#pragma once

#include <array>

template<typename Value = uint32_t, Value Vectors = 256>
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
         * UM10360 Manual
         * 6.4 Vector table remapping
         * The vector table should be located on a 256 word (1024 byte) boundary 
         * to insure alignment on LPC176x/5x family devices.
         * 
         * 0x100 = 256 * sizeof(uint32_t) = 0x400 = 1024 byte
         * Cortex M3 requires this to be aligned properly.
         */
        alignas(Vectors * sizeof(Value)) static std::array<Value, Vectors> m_VectorTable;


    public:
        enum InterruptTypes : Value; // forward declaration of enums with type/size specification.

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
         * 
         * @return InterruptVectorTable& 
         */
        static InterruptVectorTable& getInstance()
        {
            static InterruptVectorTable instance;   // Guaranteed to be destroyed.
                                                // Instantiated on first use.
            return instance;
        }


    public:
        

        /**
         * @brief 
         * 
         * @param index 
         * @param Callback Function to be called, when the Interrupt is 
         */
        bool RegisterInterruptCallback(Value index, void (*Callback)(void));

        /**
         * @brief Disable interrupt at given index
         * 
         * @param index interrupt index, as defined in the manuals: >= 0
         */
        void EnableInterrupt(Value index);

        /**
         * @brief Disable an interrupt.
         * 
         * @param index Is the index of the interrupt within the interrupt vctor table. 
         *              Should be a value greater than or equal to 0.
         */
        void DisableInterrupt(Value index);


        //void FireSoftwareInterrupt(uint32_t index);
};
