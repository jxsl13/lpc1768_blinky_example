#include "LPC17xx.h"
#include <hal/InterruptVectorTable.hpp>
#include <cstring>


enum InterruptVectorTable::InterruptTypes : ValueType {
    NonMaskableInt_IRQn = 0,    /*!< 2 Non Maskable Interrupt                         */
    MemoryManagement_IRQn,      /*!< 4 Cortex-M3 Memory Management Interrupt          */
    BusFault_IRQn,              /*!< 5 Cortex-M3 Bus Fault Interrupt                  */
    UsageFault_IRQn,            /*!< 6 Cortex-M3 Usage Fault Interrupt                */
    SVCall_IRQn,                /*!< 11 Cortex-M3 SV Call Interrupt                   */
    DebugMonitor_IRQn,          /*!< 12 Cortex-M3 Debug Monitor Interrupt             */
    PendSV_IRQn,                /*!< 14 Cortex-M3 Pend SV Interrupt                   */
    SysTick_IRQn,               /*!< 15 Cortex-M3 System Tick Interrupt               */

/******  LPC17xx Specific Interrupt Numbers *******************************************************/
    WDT_IRQn,                   /*!< Watchdog Timer Interrupt                         */
    TIMER0_IRQn,                /*!< Timer0 Interrupt                                 */
    TIMER1_IRQn,                /*!< Timer1 Interrupt                                 */
    TIMER2_IRQn,                /*!< Timer2 Interrupt                                 */
    TIMER3_IRQn,                /*!< Timer3 Interrupt                                 */
    UART0_IRQn,                 /*!< UART0 Interrupt                                  */
    UART1_IRQn,                 /*!< UART1 Interrupt                                  */
    UART2_IRQn,                 /*!< UART2 Interrupt                                  */
    UART3_IRQn,                 /*!< UART3 Interrupt                                  */
    PWM1_IRQn,                  /*!< PWM1 Interrupt                                   */
    I2C0_IRQn,                  /*!< I2C0 Interrupt                                   */
    I2C1_IRQn,                  /*!< I2C1 Interrupt                                   */
    I2C2_IRQn,                  /*!< I2C2 Interrupt                                   */
    SPI_IRQn,                   /*!< SPI Interrupt                                    */
    SSP0_IRQn,                  /*!< SSP0 Interrupt                                   */
    SSP1_IRQn,                  /*!< SSP1 Interrupt                                   */
    PLL0_IRQn,                  /*!< PLL0 Lock (Main PLL) Interrupt                   */
    RTC_IRQn,                   /*!< Real Time Clock Interrupt                        */
    EINT0_IRQn,                 /*!< External Interrupt 0 Interrupt                   */
    EINT1_IRQn,                 /*!< External Interrupt 1 Interrupt                   */
    EINT2_IRQn,                 /*!< External Interrupt 2 Interrupt                   */
    EINT3_IRQn,                 /*!< External Interrupt 3 Interrupt                   */
    ADC_IRQn,                   /*!< A/D Converter Interrupt                          */
    BOD_IRQn,                   /*!< Brown-Out Detect Interrupt                       */
    USB_IRQn,                   /*!< USB Interrupt                                    */
    CAN_IRQn,                   /*!< CAN Interrupt                                    */
    DMA_IRQn,                   /*!< General Purpose DMA Interrupt                    */
    I2S_IRQn,                   /*!< I2S Interrupt                                    */
    ENET_IRQn,                  /*!< Ethernet Interrupt                               */
    RIT_IRQn,                   /*!< Repetitive Interrupt Timer Interrupt             */
    MCPWM_IRQn,                 /*!< Motor Control PWM Interrupt                      */
    QEI_IRQn,                   /*!< Quadrature Encoder Interface Interrupt           */
    PLL1_IRQn,                  /*!< PLL1 Lock (USB PLL) Interrupt                    */
    USBActivity_IRQn,           /*!< USB Activity Interrupt(For wakeup only)          */
    CANActivity_IRQn            /*!< CAN Activity Interrupt(For wakeup only)          */
};


InterruptVectorTable::InterruptVectorTable()
{
    ValueType *vectors = (ValueType *)SCB->VTOR;

    /**
     * @brief It is possible to manipulate the vector table without actually 
     * Relocating it into RAM.
     */
    auto relocateIntoRam = [&](ValueType *VectorTable) -> void
    {
        // copy vector table to ram location
        std::memcpy(s_VectorTable.data(), VectorTable, sizeof(ValueType) * VectorsCount); 


        /* relocate vector table into RAM*/ 
        // disable global interrupts
        __disable_irq();

        /* 
            VTOR bit assignment
            ===================
            [31:30] - Reserved
            [29:8] - TBLOFF
                    Vector table base offset field. It contains bits[29:8] of the offset of the table base from the bottom of the memory map.
                    
                    Remark: Bit[29] determines whether the vector table is in the code or SRAM memory region:
                    Bit[29] is sometimes called the TBLBASE bit. 
                    • 0=code
                    • 1=SRAM. 
            [7:0] - Reserved
            
        */

        /**
         * @brief Mask only the TBLOFF part, don't touch the rest.
         */
        SCB->VTOR |= (ValueType) s_VectorTable.data() & 0x3FFFFF00;
        
        // wait for memory operations to finish
        __DSB();

        // enable interrupts again.
        __enable_irq();
    };

    relocateIntoRam(vectors);

    /**
     * @brief Creates a view onto a specific memory region without
     *          actually owning the region.
     */
    m_VectorTableView = {s_VectorTable.data(), s_VectorTable.size()};
    
}

bool InterruptVectorTable::addCallback(ValueType InterruptIndex, void (*Callback)(void))
{   
    /**
     * 
     * It is not possible to unwrap a std::function in order to extract the function pointer! 
     * Thus one can only use the raw C function pointer
     * 
     * LPC allows a range of [0:111] as InterruptIndex
     */
    if (InterruptIndex < 0 || InterruptIndex >= 111)
        return false;
    
    //s_VectorTable[InterruptIndex + NVIC_USER_IRQ_OFFSET] = reinterpret_cast<ValueType>(Callback);

    m_VectorTableView[InterruptIndex + NVIC_USER_IRQ_OFFSET] = reinterpret_cast<ValueType>(Callback);
    
    /**
     * same as:
     * 
     * NVIC_SetVector((IRQn)(InterruptIndex), (ValueType)Callback);
     */

    return true;
}

void InterruptVectorTable::enableISR(ValueType InterruptIndex)
{
    NVIC_EnableIRQ(static_cast<IRQn_Type>(InterruptIndex));
}

void InterruptVectorTable::disableISR(ValueType InterruptIndex)
{
    NVIC_DisableIRQ(static_cast<IRQn_Type>(InterruptIndex));
}

void InterruptVectorTable::triggerIRQ(ValueType InterruptIndex)
{
    /**
     * LPC allows a range of [0:111] as InterruptIndex
     */
    if (InterruptIndex < 0 || InterruptIndex > 111)
        return;
    
    /**
     * Bits 8:0 - 256 Interrupt values (binary 0b111111111 -> 0x1FF)
     * Values of bits 31:9 are reserved, so should not be read or touched
     * Manual UM10360 - Page 92
     */
    NVIC->STIR |= (0x1FF & InterruptIndex);
}

