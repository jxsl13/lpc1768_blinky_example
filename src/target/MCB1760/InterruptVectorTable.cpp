#include "LPC17xx.h"
#include <hal/InterruptVectorTable.hpp>
#include <cstring>

 


template <typename Value, Value Vectors>
enum InterruptVectorTable<Value, Vectors>::InterruptTypes : Value {
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


template <typename Value, Value Vectors>
InterruptVectorTable<Value, Vectors>::InterruptVectorTable()
{

    Value *vectors = (Value *)SCB->VTOR;

    // copy vector table to ram location
    std::memcpy(m_VectorTable.data(), vectors, sizeof(Value) * Vectors);


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
     * https://community.arm.com/developer/tools-software/tools/f/keil-forum/39482/how-to-relocate-the-cortex-m3-vector-table
     * 0x1FFFFF80 = 1111111111111111111111100000000
     */

    // given // SCB->VTOR = NVIC_VectTab | (Offset & (unsigned int)0x1FFFFF80);
    // fixed // SCB->VTOR |= ((Value) m_VectorTable.data() & (Value)0x7FFFFF00);

    // original
    SCB->VTOR = (Value) m_VectorTable.data() ;
    
    // wait for memory operations to finish
    __DSB();

    // enable interrupts again.
    __enable_irq();
}

template <typename Value, Value Vectors>
bool InterruptVectorTable<Value, Vectors>::RegisterInterruptCallback(Value index, void (*Callback)(void))
{   
    // It is not possuble to unwrap a std::function in order to extract the function pointer
    if (index < 0 || index >= Vectors)
        return false;
    
    m_VectorTable[index] = reinterpret_cast<Value>(Callback); 

    return true;
}

template <typename Value, Value Vectors>
void InterruptVectorTable<Value, Vectors>::EnableInterrupt(Value index)
{
    NVIC_EnableIRQ(static_cast<IRQn_Type>(index));
}

template <typename Value, Value Vectors>
void InterruptVectorTable<Value, Vectors>::DisableInterrupt(Value index)
{
    NVIC_DisableIRQ(static_cast<IRQn_Type>(index));
}
