#include "stm32f4xx.h"
#include <hal/InterruptVectorTable.hpp>
#include <utils/RegisterBits.hpp>
#include <cstring>
#include "InterruptTypes.hpp"


typedef void(*pFn_t)(void);


const ValidIRQTypes InterruptVectorTable::s_ValidIRQTypes = {
    82,
    { 
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 
        10, 11, 12, 13, 14, 15, 16, 17, 18, 
        19, 20, 21, 22, 23, 24, 25, 26, 27, 
        28, 29, 30, 31, 32, 33, 34, 35, 36, 
        37, 38, 39, 40, 41, 42, 43, 44, 45, 
        46, 47, 48, 49, 50, 51, 52, 53, 54, 
        55, 56, 57, 58, 59, 60, 61, 62, 63, 
        64, 65, 66, 67, 68, 69, 70, 71, 72, 
        73, 74, 75, 76, 77, 78, 79, 80, 81 
    }
};

#define RETURN_NOT_IN_VALID_RANGE() if(InterruptIndex < 0 || s_ValidIRQTypes.m_size <= InterruptIndex) return

// TODO: find all registers of all pending bits - 82 values in total
const static RegisterBits<2> s_PendingBitsMap[] = 
{
    {&EXTI->PR, EXTI0_IRQn, 1},
    {0, 0, 0},
};

/**
 * @brief Static array contains the wrapped function pointers
 */
alignas(sizeof(ValueType) * VectorsCount) static pFn_t s_VectorTable[VectorsCount];

// Template for all wrapper functions/vectors
template<ValueType IRQIndex>
void __Vector()
{
    // call wrapped function if not nullptr
    if(s_VectorTable[ NVIC_USER_IRQ_OFFSET + IRQIndex])
        s_VectorTable[ NVIC_USER_IRQ_OFFSET + IRQIndex]();

    
    volatile ValueType* Register = s_PendingBitsMap[IRQIndex].m_Register;   // register containing the pending bit.
    ValueType Bit = s_PendingBitsMap[IRQIndex].m_Bits[0];                   // bit to set in order to clear the pending bit
    ValueType SetOrClear = s_PendingBitsMap[IRQIndex].m_Bits[1];            // 1 -> set bit, 0 -> clear bit

    if (SetOrClear)
    {
        // Enable
        ENABLE(*Register, Bit);
    }
    else
    {
        // Clear Bit in Register 
        DISABLE(*Register, Bit);
    }
}


InterruptVectorTable::InterruptVectorTable()
{
    ValueType *VectorTable = (ValueType *)SCB->VTOR;

    /**
     * @brief This static array contains the wrapper functions that wrap the configurable functions
     * and after the function's execution they clear the pending bits in order to have the same behaviour as
     * all the other functions.
     */
    alignas(sizeof(ValueType) * VectorsCount) static pFn_t s_WrapperVectorTable[VectorsCount];
 

    // copy vector table to ram location into wrapper
    std::memcpy(s_WrapperVectorTable, VectorTable, sizeof(ValueType) * VectorsCount); 


    // define all wrapper functions
    s_WrapperVectorTable[ NVIC_USER_IRQ_OFFSET + s_ValidIRQTypes.m_ValidTypes[0] ] =  __Vector<0>;
    s_WrapperVectorTable[ NVIC_USER_IRQ_OFFSET + s_ValidIRQTypes.m_ValidTypes[1] ] =  __Vector<1>;
    s_WrapperVectorTable[ NVIC_USER_IRQ_OFFSET + s_ValidIRQTypes.m_ValidTypes[2] ] =  __Vector<2>;
    s_WrapperVectorTable[ NVIC_USER_IRQ_OFFSET + s_ValidIRQTypes.m_ValidTypes[3] ] =  __Vector<3>;
    s_WrapperVectorTable[ NVIC_USER_IRQ_OFFSET + s_ValidIRQTypes.m_ValidTypes[4] ] =  __Vector<4>;
    s_WrapperVectorTable[ NVIC_USER_IRQ_OFFSET + s_ValidIRQTypes.m_ValidTypes[5] ] =  __Vector<5>;
    s_WrapperVectorTable[ NVIC_USER_IRQ_OFFSET + s_ValidIRQTypes.m_ValidTypes[6] ] =  __Vector<6>;
    s_WrapperVectorTable[ NVIC_USER_IRQ_OFFSET + s_ValidIRQTypes.m_ValidTypes[7] ] =  __Vector<7>;
    s_WrapperVectorTable[ NVIC_USER_IRQ_OFFSET + s_ValidIRQTypes.m_ValidTypes[8] ] =  __Vector<8>;
    s_WrapperVectorTable[ NVIC_USER_IRQ_OFFSET + s_ValidIRQTypes.m_ValidTypes[9] ] =  __Vector<9>;
    s_WrapperVectorTable[ NVIC_USER_IRQ_OFFSET + s_ValidIRQTypes.m_ValidTypes[10] ] =  __Vector<10>;
    s_WrapperVectorTable[ NVIC_USER_IRQ_OFFSET + s_ValidIRQTypes.m_ValidTypes[11] ] =  __Vector<11>;
    s_WrapperVectorTable[ NVIC_USER_IRQ_OFFSET + s_ValidIRQTypes.m_ValidTypes[12] ] =  __Vector<12>;
    s_WrapperVectorTable[ NVIC_USER_IRQ_OFFSET + s_ValidIRQTypes.m_ValidTypes[13] ] =  __Vector<13>;
    s_WrapperVectorTable[ NVIC_USER_IRQ_OFFSET + s_ValidIRQTypes.m_ValidTypes[14] ] =  __Vector<14>;
    s_WrapperVectorTable[ NVIC_USER_IRQ_OFFSET + s_ValidIRQTypes.m_ValidTypes[15] ] =  __Vector<15>;
    s_WrapperVectorTable[ NVIC_USER_IRQ_OFFSET + s_ValidIRQTypes.m_ValidTypes[16] ] =  __Vector<16>;
    s_WrapperVectorTable[ NVIC_USER_IRQ_OFFSET + s_ValidIRQTypes.m_ValidTypes[17] ] =  __Vector<17>;
    s_WrapperVectorTable[ NVIC_USER_IRQ_OFFSET + s_ValidIRQTypes.m_ValidTypes[18] ] =  __Vector<18>;
    s_WrapperVectorTable[ NVIC_USER_IRQ_OFFSET + s_ValidIRQTypes.m_ValidTypes[19] ] =  __Vector<19>;
    s_WrapperVectorTable[ NVIC_USER_IRQ_OFFSET + s_ValidIRQTypes.m_ValidTypes[20] ] =  __Vector<20>;
    s_WrapperVectorTable[ NVIC_USER_IRQ_OFFSET + s_ValidIRQTypes.m_ValidTypes[21] ] =  __Vector<21>;
    s_WrapperVectorTable[ NVIC_USER_IRQ_OFFSET + s_ValidIRQTypes.m_ValidTypes[22] ] =  __Vector<22>;
    s_WrapperVectorTable[ NVIC_USER_IRQ_OFFSET + s_ValidIRQTypes.m_ValidTypes[23] ] =  __Vector<23>;
    s_WrapperVectorTable[ NVIC_USER_IRQ_OFFSET + s_ValidIRQTypes.m_ValidTypes[24] ] =  __Vector<24>;
    s_WrapperVectorTable[ NVIC_USER_IRQ_OFFSET + s_ValidIRQTypes.m_ValidTypes[25] ] =  __Vector<25>;
    s_WrapperVectorTable[ NVIC_USER_IRQ_OFFSET + s_ValidIRQTypes.m_ValidTypes[26] ] =  __Vector<26>;
    s_WrapperVectorTable[ NVIC_USER_IRQ_OFFSET + s_ValidIRQTypes.m_ValidTypes[27] ] =  __Vector<27>;
    s_WrapperVectorTable[ NVIC_USER_IRQ_OFFSET + s_ValidIRQTypes.m_ValidTypes[28] ] =  __Vector<28>;
    s_WrapperVectorTable[ NVIC_USER_IRQ_OFFSET + s_ValidIRQTypes.m_ValidTypes[29] ] =  __Vector<29>;
    s_WrapperVectorTable[ NVIC_USER_IRQ_OFFSET + s_ValidIRQTypes.m_ValidTypes[30] ] =  __Vector<30>;
    s_WrapperVectorTable[ NVIC_USER_IRQ_OFFSET + s_ValidIRQTypes.m_ValidTypes[31] ] =  __Vector<31>;
    s_WrapperVectorTable[ NVIC_USER_IRQ_OFFSET + s_ValidIRQTypes.m_ValidTypes[32] ] =  __Vector<32>;
    s_WrapperVectorTable[ NVIC_USER_IRQ_OFFSET + s_ValidIRQTypes.m_ValidTypes[33] ] =  __Vector<33>;
    s_WrapperVectorTable[ NVIC_USER_IRQ_OFFSET + s_ValidIRQTypes.m_ValidTypes[34] ] =  __Vector<34>;
    s_WrapperVectorTable[ NVIC_USER_IRQ_OFFSET + s_ValidIRQTypes.m_ValidTypes[35] ] =  __Vector<35>;
    s_WrapperVectorTable[ NVIC_USER_IRQ_OFFSET + s_ValidIRQTypes.m_ValidTypes[36] ] =  __Vector<36>;
    s_WrapperVectorTable[ NVIC_USER_IRQ_OFFSET + s_ValidIRQTypes.m_ValidTypes[37] ] =  __Vector<37>;
    s_WrapperVectorTable[ NVIC_USER_IRQ_OFFSET + s_ValidIRQTypes.m_ValidTypes[38] ] =  __Vector<38>;
    s_WrapperVectorTable[ NVIC_USER_IRQ_OFFSET + s_ValidIRQTypes.m_ValidTypes[39] ] =  __Vector<39>;
    s_WrapperVectorTable[ NVIC_USER_IRQ_OFFSET + s_ValidIRQTypes.m_ValidTypes[40] ] =  __Vector<40>;
    s_WrapperVectorTable[ NVIC_USER_IRQ_OFFSET + s_ValidIRQTypes.m_ValidTypes[41] ] =  __Vector<41>;
    s_WrapperVectorTable[ NVIC_USER_IRQ_OFFSET + s_ValidIRQTypes.m_ValidTypes[42] ] =  __Vector<42>;
    s_WrapperVectorTable[ NVIC_USER_IRQ_OFFSET + s_ValidIRQTypes.m_ValidTypes[43] ] =  __Vector<43>;
    s_WrapperVectorTable[ NVIC_USER_IRQ_OFFSET + s_ValidIRQTypes.m_ValidTypes[44] ] =  __Vector<44>;
    s_WrapperVectorTable[ NVIC_USER_IRQ_OFFSET + s_ValidIRQTypes.m_ValidTypes[45] ] =  __Vector<45>;
    s_WrapperVectorTable[ NVIC_USER_IRQ_OFFSET + s_ValidIRQTypes.m_ValidTypes[46] ] =  __Vector<46>;
    s_WrapperVectorTable[ NVIC_USER_IRQ_OFFSET + s_ValidIRQTypes.m_ValidTypes[47] ] =  __Vector<47>;
    s_WrapperVectorTable[ NVIC_USER_IRQ_OFFSET + s_ValidIRQTypes.m_ValidTypes[48] ] =  __Vector<48>;
    s_WrapperVectorTable[ NVIC_USER_IRQ_OFFSET + s_ValidIRQTypes.m_ValidTypes[49] ] =  __Vector<49>;
    s_WrapperVectorTable[ NVIC_USER_IRQ_OFFSET + s_ValidIRQTypes.m_ValidTypes[50] ] =  __Vector<50>;
    s_WrapperVectorTable[ NVIC_USER_IRQ_OFFSET + s_ValidIRQTypes.m_ValidTypes[51] ] =  __Vector<51>;
    s_WrapperVectorTable[ NVIC_USER_IRQ_OFFSET + s_ValidIRQTypes.m_ValidTypes[52] ] =  __Vector<52>;
    s_WrapperVectorTable[ NVIC_USER_IRQ_OFFSET + s_ValidIRQTypes.m_ValidTypes[53] ] =  __Vector<53>;
    s_WrapperVectorTable[ NVIC_USER_IRQ_OFFSET + s_ValidIRQTypes.m_ValidTypes[54] ] =  __Vector<54>;
    s_WrapperVectorTable[ NVIC_USER_IRQ_OFFSET + s_ValidIRQTypes.m_ValidTypes[55] ] =  __Vector<55>;
    s_WrapperVectorTable[ NVIC_USER_IRQ_OFFSET + s_ValidIRQTypes.m_ValidTypes[56] ] =  __Vector<56>;
    s_WrapperVectorTable[ NVIC_USER_IRQ_OFFSET + s_ValidIRQTypes.m_ValidTypes[57] ] =  __Vector<57>;
    s_WrapperVectorTable[ NVIC_USER_IRQ_OFFSET + s_ValidIRQTypes.m_ValidTypes[58] ] =  __Vector<58>;
    s_WrapperVectorTable[ NVIC_USER_IRQ_OFFSET + s_ValidIRQTypes.m_ValidTypes[59] ] =  __Vector<59>;
    s_WrapperVectorTable[ NVIC_USER_IRQ_OFFSET + s_ValidIRQTypes.m_ValidTypes[60] ] =  __Vector<60>;
    s_WrapperVectorTable[ NVIC_USER_IRQ_OFFSET + s_ValidIRQTypes.m_ValidTypes[61] ] =  __Vector<61>;
    s_WrapperVectorTable[ NVIC_USER_IRQ_OFFSET + s_ValidIRQTypes.m_ValidTypes[62] ] =  __Vector<62>;
    s_WrapperVectorTable[ NVIC_USER_IRQ_OFFSET + s_ValidIRQTypes.m_ValidTypes[63] ] =  __Vector<63>;
    s_WrapperVectorTable[ NVIC_USER_IRQ_OFFSET + s_ValidIRQTypes.m_ValidTypes[64] ] =  __Vector<64>;
    s_WrapperVectorTable[ NVIC_USER_IRQ_OFFSET + s_ValidIRQTypes.m_ValidTypes[65] ] =  __Vector<65>;
    s_WrapperVectorTable[ NVIC_USER_IRQ_OFFSET + s_ValidIRQTypes.m_ValidTypes[66] ] =  __Vector<66>;
    s_WrapperVectorTable[ NVIC_USER_IRQ_OFFSET + s_ValidIRQTypes.m_ValidTypes[67] ] =  __Vector<67>;
    s_WrapperVectorTable[ NVIC_USER_IRQ_OFFSET + s_ValidIRQTypes.m_ValidTypes[68] ] =  __Vector<68>;
    s_WrapperVectorTable[ NVIC_USER_IRQ_OFFSET + s_ValidIRQTypes.m_ValidTypes[69] ] =  __Vector<69>;
    s_WrapperVectorTable[ NVIC_USER_IRQ_OFFSET + s_ValidIRQTypes.m_ValidTypes[70] ] =  __Vector<70>;
    s_WrapperVectorTable[ NVIC_USER_IRQ_OFFSET + s_ValidIRQTypes.m_ValidTypes[71] ] =  __Vector<71>;
    s_WrapperVectorTable[ NVIC_USER_IRQ_OFFSET + s_ValidIRQTypes.m_ValidTypes[72] ] =  __Vector<72>;
    s_WrapperVectorTable[ NVIC_USER_IRQ_OFFSET + s_ValidIRQTypes.m_ValidTypes[73] ] =  __Vector<73>;
    s_WrapperVectorTable[ NVIC_USER_IRQ_OFFSET + s_ValidIRQTypes.m_ValidTypes[74] ] =  __Vector<74>;
    s_WrapperVectorTable[ NVIC_USER_IRQ_OFFSET + s_ValidIRQTypes.m_ValidTypes[75] ] =  __Vector<75>;
    s_WrapperVectorTable[ NVIC_USER_IRQ_OFFSET + s_ValidIRQTypes.m_ValidTypes[76] ] =  __Vector<76>;
    s_WrapperVectorTable[ NVIC_USER_IRQ_OFFSET + s_ValidIRQTypes.m_ValidTypes[77] ] =  __Vector<77>;
    s_WrapperVectorTable[ NVIC_USER_IRQ_OFFSET + s_ValidIRQTypes.m_ValidTypes[78] ] =  __Vector<78>;
    s_WrapperVectorTable[ NVIC_USER_IRQ_OFFSET + s_ValidIRQTypes.m_ValidTypes[79] ] =  __Vector<79>;
    s_WrapperVectorTable[ NVIC_USER_IRQ_OFFSET + s_ValidIRQTypes.m_ValidTypes[80] ] =  __Vector<80>;
    s_WrapperVectorTable[ NVIC_USER_IRQ_OFFSET + s_ValidIRQTypes.m_ValidTypes[81] ] =  __Vector<81>;
    
    // dummy handler, that does nothing.
    pFn_t DefaultHandler = []() -> void {};

    // fill the rest of the wrapper array with dummy handlers.
    for (ValueType i = (NVIC_USER_IRQ_OFFSET + s_ValidIRQTypes.m_ValidTypes[81] + 1); i < VectorsCount; i++)
    {
       s_WrapperVectorTable[i] = DefaultHandler;
    }

    // Make wrapped fuctions do be handled by the DefaultHandler.
    for (ValueType i = 0; i < VectorsCount; i++)
    {
        s_VectorTable[i] = DefaultHandler;
    }    
   
    /* relocate vector table into RAM*/ 
    // disable global interrupts
    disableIRQ();

    /**
     * @brief Mask only the TBLOFF part, don't touch the rest.
     * NVIC jumps onto WrapperFunctions
     */
    SCB->VTOR |= reinterpret_cast<ValueType>(s_WrapperVectorTable) & 0x3FFFFF00;
    
    // point to the static vector, as that vector stays the same forever.
    m_VectorTable = reinterpret_cast<ValueType*>(s_VectorTable);

    
    // wait for memory operations to finish
    __DSB();

    /**
     * We do not want to enable interrupts after the creation of the InterruptVectorTable instance,
     * because we need to configure the interrupt before enabling them globally.
     */
}

InterruptVectorTable::~InterruptVectorTable()
{
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

bool InterruptVectorTable::setCallback(ValueType InterruptIndex, void (*Callback)(void))
{   
    /**
     * 
     * It is not possible to unwrap a std::function in order to extract the function pointer! 
     * Thus one can only use the raw C function pointer
     * 
     * LPC allows a range of [0:111] as InterruptIndex
     * 
     * we allow even less, as we only have a specific range of IRQ Indices defined.
     */
    if (InterruptIndex < 0 || s_ValidIRQTypes.m_size <= InterruptIndex)
        return false;
    
    /**
     * @brief No offset needed here, as we start in the 
     * 
     */
    m_VectorTable[NVIC_USER_IRQ_OFFSET + InterruptIndex] = reinterpret_cast<ValueType>(Callback);
        
    return true;
}

void InterruptVectorTable::enableISR(ValueType InterruptIndex)
{
    RETURN_NOT_IN_VALID_RANGE();
   
    NVIC_EnableIRQ(static_cast<IRQn_Type>(InterruptIndex));
}

void InterruptVectorTable::disableISR(ValueType InterruptIndex)
{
    RETURN_NOT_IN_VALID_RANGE();

    NVIC_DisableIRQ(static_cast<IRQn_Type>(InterruptIndex));
}

void InterruptVectorTable::triggerIRQ(ValueType InterruptIndex)
{
    RETURN_NOT_IN_VALID_RANGE();
    
    /**
     * Bits 8:0 - 256 Interrupt values (binary 0b111111111 -> 0x1FF)
     * Values of bits 31:9 are reserved, so should not be read or touched
     * Manual UM10360 - Page 92
     */
    NVIC->STIR |= (0x1FF & InterruptIndex);
}

