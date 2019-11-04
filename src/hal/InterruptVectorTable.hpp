#pragma once

#include <array>
#include <span-lite/include/nonstd/span.hpp>

using ValueType = uint32_t;
constexpr ValueType VectorsCount = 256;


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
         * 
         * Since C++17 a static member variables may be defined with the inline keyword directly
         * in the header file: inline static m_Whatever
         * https://stackoverflow.com/questions/185844/how-to-initialize-private-static-members-in-c
         * 
         * The inline part allows for this to be compiled only once! and not in every .cpp file this might be included in.
         * 
         * If the Interrupt Vector Table needs to be relocated, we relocate it into RAM using this array.
         */
        alignas(VectorsCount * sizeof(ValueType)) inline static std::array<ValueType, VectorsCount> s_VectorTable;
        
        /**
         * @brief If the InterruptVectorTable cannot be relocated, 
         * we abstract it using the View
         * std::span<> will be added in C++20 - This header only dependency will automatically support that.
         */
        nonstd::span<ValueType> m_VectorTableView;


    public:
        enum InterruptTypes : ValueType; // forward declaration of enums with type/size specification.

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
         * @param InterruptNumber 
         * @param Callback Function to be called, when the Interrupt is 
         */
        bool addCallback(ValueType InterruptNumber, void (*Callback)(void));

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

