#pragma once

#include <functional>
#include <vector>

class InterruptVectorTable
{
    // https://stackoverflow.com/questions/1008019/c-singleton-design-pattern
    public:
        enum InterruptTypes : uint32_t; // forward declaration of enums with type/size specification.

        static InterruptVectorTable& GetInstance()
        {
            static InterruptVectorTable instance;   // Guaranteed to be destroyed.
                                                // Instantiated on first use.
            return instance;
        }
    private:
        static std::vector<uint32_t> m_VectorTable;

        InterruptVectorTable();

    public:
        InterruptVectorTable(InterruptVectorTable const&)   = delete;
        void operator=(InterruptVectorTable const&)         = delete;

        /**
         * @brief 
         * 
         * @param index 
         * @param Callback Function to be called, when the Interrupt is 
         * @return true If the registration succeeded
         * @return false If the registration was not successful, due to e.g. an invalid index
         */
        bool RegisterInterruptCallback(uint32_t index, void (*Callback)(void));


        //void FireSoftwareInterrupt(uint32_t index);
};



