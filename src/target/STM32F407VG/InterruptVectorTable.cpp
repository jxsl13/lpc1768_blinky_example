#include "Interrupt.hpp"
#include <utils/RegisterBits.hpp>
#include <cstring>


typedef void(*pFn_t)(void);



InterruptVectorTable::InterruptVectorTable()
{

    // disable global interrupts
    disableIRQ();

    ValueType *VectorTable = (ValueType *)SCB->VTOR;

    /**
     * @brief LPC1768 for example needs this alignment of the vector table.
     * Also LPC1768 needs the vector table to be relocated into ram in order to dynamically
     * change the function pointers.
     */
    alignas(sizeof(ValueType) * Alignment) static ValueType s_VectorTable[VectorsCount];
    

    /* relocate vector table into RAM*/ 
    std::memcpy(s_VectorTable, VectorTable, sizeof(ValueType) * VectorsCount); 


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
    SCB->VTOR = ((ValueType)s_VectorTable & 0x3FFFFE00);
    
    // point to the static vector, as that vector stays the same forever.
    m_VectorTable = s_VectorTable;
    
    // wait for memory operations to finish
    __DSB();

    /**
     * We do not want to enable interrupts after the creation of the InterruptVectorTable instance,
     * because we need to configure the interrupt before enabling them globally.
     */
}

void InterruptVectorTable::enableIRQ()
{
    // enable interrupts again.
    __enable_irq();
}

void InterruptVectorTable::disableIRQ()
{
    // disable global interrupts
    __disable_irq();
}

bool InterruptVectorTable::isEnabled()
{
    /**
     * PRIMASK is a 1-bit-wide interrupt mask register. When set, it blocks all interrupts apart from the non-maskable 
     * interrupt (NMI) and the hard fault exception. The PRIMASK prevents activation of all exceptions with configurable 
     * priority.
     * https://www.keil.com/pack/doc/CMSIS/Core/html/group__Core__Register__gr.html#ga799b5d9a2ae75e459264c8512c7c0e02
     * 
     * 1 -> global interrupts are enabled
     * 0 -> global interrupts are disabled
     * */
    return __get_PRIMASK() == 0;
}

void InterruptVectorTable::setCallback(IRQType InterruptIndex, void (*Callback)(void))
{   
    /**
     * It is not possible to unwrap a std::function in order to extract the function pointer! 
     * Thus one can only use the raw C function pointer
     */
    ValueType index = static_cast<ValueType>(InterruptIndex);
    m_VectorTable[index + NVIC_USER_IRQ_OFFSET] = reinterpret_cast<ValueType>(Callback);
    
    /**
     * Same as:
     * NVIC_SetVector((IRQn_Type)(InterruptIndex), (ValueType)Callback);
     */
}

void InterruptVectorTable::enableISR(IRQType InterruptIndex)
{
    NVIC_EnableIRQ(static_cast<IRQn_Type>(InterruptIndex));
}

void InterruptVectorTable::disableISR(IRQType InterruptIndex)
{
    NVIC_DisableIRQ(static_cast<IRQn_Type>(InterruptIndex));
}

bool InterruptVectorTable::isEnabled(IRQType InterruptIndex)
{
    return NVIC_GetEnableIRQ(static_cast<IRQn_Type>(InterruptIndex));
}


void InterruptVectorTable::triggerIRQ(IRQType InterruptIndex)
{

    /**
     * Bits 8:0 - 256 Interrupt values (binary 0b111111111 -> 0x1FF)
     * Values of bits 31:9 are reserved, so should not be read or touched
     * Manual UM10360 - Page 92
     */
    ValueType index = static_cast<ValueType>(InterruptIndex);
    NVIC->STIR |= (0x1FF & index);
}


