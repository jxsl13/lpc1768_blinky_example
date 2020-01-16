#include <cstring>

#include <utils/BitMacros.hpp>
#include <utils/RegisterBits.hpp>

#include <hal/Interrupt.hpp>
#include "InterruptType.hpp"
#include "device.hpp"

namespace holmes
{
namespace internal
{
/**
 * @brief Specialization of the InterruptVectorTable for the DeviceAtMega328p
 * 
 * @tparam  
 */
template <>
class InterruptVectorTable<DeviceSTM32F407VG, uint32_t, IRQType>
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
        // disable global interrupts
        disableIRQ();

        uint32_t *VectorTable = (uint32_t *)SCB->VTOR;

        /**
         * @brief LPC1768 for example needs this alignment of the vector table.
         * Also LPC1768 needs the vector table to be relocated into ram in order to dynamically
         * change the function pointers.
         * 
         * needs to be aligned at a 256 word bounary in ram
         */
        alignas(sizeof(uint32_t) * 256) static uint32_t s_VectorTable[DeviceSTM32F407VG::s_NumInterruptVectors];

        /* relocate vector table into RAM*/
        std::memcpy(s_VectorTable, VectorTable, sizeof(uint32_t) * DeviceSTM32F407VG::s_NumInterruptVectors);

        /* 
            VTOR bit assignment
            ===================
            [31:30] - Reserved
            [29:9] - TBLOFF
                    Vector table base offset field. It contains bits[29:9] of the offset of the table base from the bottom of the memory map.
                    
                    Remark: Bit[29] determines whether the vector table is in the code or SRAM memory region:
                    Bit[29] is sometimes called the TBLBASE bit. 
                    • 0=Code
                    • 1=SRAM. 
            [7:0] - Reserved
            
        */

        /**
         * @brief Mask only the TBLOFF part, don't touch the rest.
         */
        SCB->VTOR = ((uint32_t)s_VectorTable & 0x3FFFFE00);

        // point to the static vector, as that vector stays the same forever.
        m_VectorTable = s_VectorTable;

        // wait for memory operations to finish
        __DSB();

        /**
         * We do not want to enable interrupts after the creation of the InterruptVectorTable instance,
         * because we need to configure the interrupt before enabling them globally.
         */
    }

    /**
     * @brief This vector pointer needs to point to the Vector Table.
     * The vector Table needs to have the explicit size of DeviceAtMega328p::s_NumInterruptVectors of the uint8_t.
     * The behaviour is undefined if the vector is accessed with offsets that are not within the
     * range [0:DeviceAtMega328p::s_NumInterruptVectors[
     */
    uint32_t *m_VectorTable;

   public:
    static auto getInstance() -> InterruptVectorTable &
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
         * It is not possible to unwrap a std::function in order to extract the function pointer! 
         * Thus one can only use the raw C function pointer
         */
        uint32_t index = static_cast<uint32_t>(InterruptIndex);
        m_VectorTable[index + NVIC_USER_IRQ_OFFSET] = reinterpret_cast<uint32_t>(Callback);
    }

    auto enableIRQ() -> void
    {
        // enable interrupts globally.
        __enable_irq();
    }

    auto disableIRQ() -> void
    {
        // disable global interrupts
        __disable_irq();
    }

    auto isEnabled() -> bool
    {
        /**
         * PRIMASK is a 1-bit-wide interrupt mask register. When set, it blocks all interrupts apart from the non-maskable 
         * interrupt (NMI) and the hard fault exception. The PRIMASK prevents activation of all exceptions with configurable 
         * priority.
         * https://www.keil.com/pack/doc/CMSIS/Core/html/group__Core__Register__gr.html#ga799b5d9a2ae75e459264c8512c7c0e02
         * 
         * 1 -> global interrupts are enabled
         * 0 -> global interrupts are disabled
         */
        return __get_PRIMASK() == 0;
    }

    auto enableISR(IRQType InterruptIndex) -> void
    {
        NVIC_EnableIRQ(static_cast<IRQn_Type>(InterruptIndex));
    }

    auto disableISR(IRQType InterruptIndex) -> void
    {
        NVIC_DisableIRQ(static_cast<IRQn_Type>(InterruptIndex));
    }

    auto isEnabled(IRQType InterruptIndex) -> bool
    {
        return NVIC_GetEnableIRQ(static_cast<IRQn_Type>(InterruptIndex));
    }

    auto triggerIRQ(IRQType InterruptIndex) -> void
    {
        /**
         * Bits 8:0 - 256 Interrupt values (binary 0b111111111 -> 0x1FF)
         * Values of bits 31:9 are reserved, so should not be read or touched
         * Manual UM10360 - Page 92
         */
        uint32_t index = static_cast<uint32_t>(InterruptIndex);
        NVIC->STIR |= (0x1FF & index);
    }

    auto waitForIRQ() -> void
    {
        // wait for interrupt OR event
        // https://community.arm.com/developer/ip-products/processors/f/cortex-a-forum/5695/difference-between-wfi-and-wfe
        __enable_irq();


        // if an event triggers this, the pending bits of the triggering interrupt request needs to be reset
        // https://community.st.com/s/question/0D50X00009XkfSY/stm32f401-stop-mode-entry
        __WFE();
        __WFE();
    }
};

} // namespace internal
} // namespace holmes