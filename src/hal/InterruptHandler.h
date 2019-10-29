#pragma once

#include <functional>
#include <vector>


class InterruptHandler
{
    // https://stackoverflow.com/questions/1008019/c-singleton-design-pattern
    public:
        enum InterruptTypes : uint32_t; // forward declaration of enums with type/size specification.

        static InterruptHandler& GetInstance()
        {
            static InterruptHandler instance;   // Guaranteed to be destroyed.
                                                // Instantiated on first use.
            return instance;
        }
    private:
        std::vector<uint32_t> m_VectorTable;

        InterruptHandler();

    public:
        InterruptHandler(InterruptHandler const&)   = delete;
        void operator=(InterruptHandler const&)     = delete;

        /**
         * @brief 
         * 
         * @param index 
         * @param Callback Function to be called, when the Interrupt is 
         * @return true If the registration succeeded
         * @return false If the registration was not successful, due to e.g. an invalid index
         */
        bool RegisterInterruptHandler(InterruptTypes index, std::function<void(void)> Callback);

};



