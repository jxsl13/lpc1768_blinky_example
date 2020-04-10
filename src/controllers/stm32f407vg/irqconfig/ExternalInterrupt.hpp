#pragma once

#include <hal/irqconfig/ExternalInterrupt.hpp>
#include "../device.hpp"

namespace holmes
{
namespace internal
{

    enum class ExtIntIndexType : DeviceSTM32F407VG::ValueTypeUnsigned;
    enum class ExtIntTriggerType : DeviceSTM32F407VG::ValueTypeUnsigned;

    template<>
    class ExternalInterrupt<DeviceSTM32F407VG, ExtIntIndexType, ExtIntTriggerType>
    {
        // only used internally
        using ValueType =   DeviceSTM32F407VG::ValueTypeUnsigned;
    public:
        using IndexType =   ExtIntIndexType;
        using TriggerType = ExtIntTriggerType;

        ExternalInterrupt(IndexType InterruptIndex, TriggerType Trigger);
        auto getTrigger() const -> TriggerType;
        auto applyTo(IndexType InterruptIndex) const -> void;
        auto apply() const -> void;
        static auto clearPendingBitOf(IndexType InterruptIndex) -> void;
        auto clearPendingBit() -> void;
    
    private:
        IndexType m_Index;
        ValueType m_Trigger;
    };

    /**
     * @brief The index encodes multiple parameters
     * 0x[00][00][0600]
     * 
     * The first [[E][I]] : Event Toggle, Interrupt Toggle
     * The second pair of [] interrupt/event index of the interrupt(EINT0 -> 0, EINT1 -> 1,...)
     * The third pair of [] defines a "Line mask", that corresponds to the on pages 289 - 293 defined
     * bit offsets, that configure, what line the specific interrupt/event is connected to.
     * 
     */
    enum class ExtIntIndexType : DeviceSTM32F407VG::ValueTypeUnsigned {
    
        // External Interrupts
        IDX_EXTI0_PA0 = 0x01000000,
        IDX_EXTI0_PB0 = 0x01000001,
        IDX_EXTI0_PC0 = 0x01000002,
        IDX_EXTI0_PD0 = 0x01000003,
        IDX_EXTI0_PE0 = 0x01000004,
        IDX_EXTI0_PF0 = 0x01000005,
        IDX_EXTI0_PG0 = 0x01000006,
        IDX_EXTI0_PH0 = 0x01000007,
        IDX_EXTI0_PI0 = 0x01000008,

        IDX_EXTI1_PA1 = 0x01010000,
        IDX_EXTI1_PB1 = 0x01010010,
        IDX_EXTI1_PC1 = 0x01010020,
        IDX_EXTI1_PD1 = 0x01010030,
        IDX_EXTI1_PE1 = 0x01010040,
        IDX_EXTI1_PF1 = 0x01010050,
        IDX_EXTI1_PG1 = 0x01010060,
        IDX_EXTI1_PH1 = 0x01010070,
        IDX_EXTI1_PI1 = 0x01010080,

        IDX_EXTI2_PA2 = 0x01020000,
        IDX_EXTI2_PB2 = 0x01020100,
        IDX_EXTI2_PC2 = 0x01020200,
        IDX_EXTI2_PD2 = 0x01020300,
        IDX_EXTI2_PE2 = 0x01020400,
        IDX_EXTI2_PF2 = 0x01020500,
        IDX_EXTI2_PG2 = 0x01020600,
        IDX_EXTI2_PH2 = 0x01020700,
        IDX_EXTI2_PI2 = 0x01020800,

        IDX_EXTI3_PA3 = 0x01030000,
        IDX_EXTI3_PB3 = 0x01031000,
        IDX_EXTI3_PC3 = 0x01032000,
        IDX_EXTI3_PD3 = 0x01033000,
        IDX_EXTI3_PE3 = 0x01034000,
        IDX_EXTI3_PF3 = 0x01035000,
        IDX_EXTI3_PG3 = 0x01036000,
        IDX_EXTI3_PH3 = 0x01037000,
        IDX_EXTI3_PI3 = 0x01038000,


        IDX_EXTI4_PA4 = 0x01040000,
        IDX_EXTI4_PB4 = 0x01040001,
        IDX_EXTI4_PC4 = 0x01040002,
        IDX_EXTI4_PD4 = 0x01040003,
        IDX_EXTI4_PE4 = 0x01040004,
        IDX_EXTI4_PF4 = 0x01040005,
        IDX_EXTI4_PG4 = 0x01040006,
        IDX_EXTI4_PH4 = 0x01040007,
        IDX_EXTI4_PI4 = 0x01040008,

        IDX_EXTI5_PA5 = 0x01050000,
        IDX_EXTI5_PB5 = 0x01050010,
        IDX_EXTI5_PC5 = 0x01050020,
        IDX_EXTI5_PD5 = 0x01050030,
        IDX_EXTI5_PE5 = 0x01050040,
        IDX_EXTI5_PF5 = 0x01050050,
        IDX_EXTI5_PG5 = 0x01050060,
        IDX_EXTI5_PH5 = 0x01050070,
        IDX_EXTI5_PI5 = 0x01050080,

        IDX_EXTI6_PA6 = 0x01060000,
        IDX_EXTI6_PB6 = 0x01060100,
        IDX_EXTI6_PC6 = 0x01060200,
        IDX_EXTI6_PD6 = 0x01060300,
        IDX_EXTI6_PE6 = 0x01060400,
        IDX_EXTI6_PF6 = 0x01060500,
        IDX_EXTI6_PG6 = 0x01060600,
        IDX_EXTI6_PH6 = 0x01060700,
        IDX_EXTI6_PI6 = 0x01060800,

        IDX_EXTI7_PA7 = 0x01070000,
        IDX_EXTI7_PB7 = 0x01071000,
        IDX_EXTI7_PC7 = 0x01072000,
        IDX_EXTI7_PD7 = 0x01073000,
        IDX_EXTI7_PE7 = 0x01074000,
        IDX_EXTI7_PF7 = 0x01075000,
        IDX_EXTI7_PG7 = 0x01076000,
        IDX_EXTI7_PH7 = 0x01077000,
        IDX_EXTI7_PI7 = 0x01078000,

        IDX_EXTI8_PA8 = 0x01080000,
        IDX_EXTI8_PB8 = 0x01080001,
        IDX_EXTI8_PC8 = 0x01080002,
        IDX_EXTI8_PD8 = 0x01080003,
        IDX_EXTI8_PE8 = 0x01080004,
        IDX_EXTI8_PF8 = 0x01080005,
        IDX_EXTI8_PG8 = 0x01080006,
        IDX_EXTI8_PH8 = 0x01080007,
        IDX_EXTI8_PI8 = 0x01080008,
        
        IDX_EXTI9_PA9 = 0x01090000,
        IDX_EXTI9_PB9 = 0x01090010,
        IDX_EXTI9_PC9 = 0x01090020,
        IDX_EXTI9_PD9 = 0x01090030,
        IDX_EXTI9_PE9 = 0x01090040,
        IDX_EXTI9_PF9 = 0x01090050,
        IDX_EXTI9_PG9 = 0x01090060,
        IDX_EXTI9_PH9 = 0x01090070,
        IDX_EXTI9_PI9 = 0x01090080,
        
        IDX_EXTI10_PA10 = 0x010A0000,
        IDX_EXTI10_PB10 = 0x010A0100,
        IDX_EXTI10_PC10 = 0x010A0200,
        IDX_EXTI10_PD10 = 0x010A0300,
        IDX_EXTI10_PE10 = 0x010A0400,
        IDX_EXTI10_PF10 = 0x010A0500,
        IDX_EXTI10_PG10 = 0x010A0600,
        IDX_EXTI10_PH10 = 0x010A0700,
        IDX_EXTI10_PI10 = 0x010A0800,
        
        IDX_EXTI11_PA11 = 0x010B0000,
        IDX_EXTI11_PB11 = 0x010B1000,
        IDX_EXTI11_PC11 = 0x010B2000,
        IDX_EXTI11_PD11 = 0x010B3000,
        IDX_EXTI11_PE11 = 0x010B4000,
        IDX_EXTI11_PF11 = 0x010B5000,
        IDX_EXTI11_PG11 = 0x010B6000,
        IDX_EXTI11_PH11 = 0x010B7000,
        IDX_EXTI11_PI11 = 0x010B8000,
        
        IDX_EXTI12_PA12 = 0x010C0000,
        IDX_EXTI12_PB12 = 0x010C0001,
        IDX_EXTI12_PC12 = 0x010C0002,
        IDX_EXTI12_PD12 = 0x010C0003,
        IDX_EXTI12_PE12 = 0x010C0004,
        IDX_EXTI12_PF12 = 0x010C0005,
        IDX_EXTI12_PG12 = 0x010C0006,
        IDX_EXTI12_PH12 = 0x010C0007,
        IDX_EXTI12_PI12 = 0x010C0008,

        IDX_EXTI13_PA13 = 0x010D0000,
        IDX_EXTI13_PB13 = 0x010D0010,
        IDX_EXTI13_PC13 = 0x010D0020,
        IDX_EXTI13_PD13 = 0x010D0030,
        IDX_EXTI13_PE13 = 0x010D0040,
        IDX_EXTI13_PF13 = 0x010D0050,
        IDX_EXTI13_PG13 = 0x010D0060,
        IDX_EXTI13_PH13 = 0x010D0070,
        IDX_EXTI13_PI13 = 0x010D0080,

        IDX_EXTI14_PA14 = 0x010E0000,
        IDX_EXTI14_PB14 = 0x010E0100,
        IDX_EXTI14_PC14 = 0x010E0200,
        IDX_EXTI14_PD14 = 0x010E0300,
        IDX_EXTI14_PE14 = 0x010E0400,
        IDX_EXTI14_PF14 = 0x010E0500,
        IDX_EXTI14_PG14 = 0x010E0600,
        IDX_EXTI14_PH14 = 0x010E0700,
        IDX_EXTI14_PI14 = 0x010E0800,

        IDX_EXTI15_PA15 = 0x010F0000,
        IDX_EXTI15_PB15 = 0x010F1000,
        IDX_EXTI15_PC15 = 0x010F2000,
        IDX_EXTI15_PD15 = 0x010F3000,
        IDX_EXTI15_PE15 = 0x010F4000,
        IDX_EXTI15_PF15 = 0x010F5000,
        IDX_EXTI15_PG15 = 0x010F6000,
        IDX_EXTI15_PH15 = 0x010F7000,
        IDX_EXTI15_PI15 = 0x010F8000,
    /*
        IDX_EXTI16 = 0x00100000,
        IDX_EXTI17 = 0x00110000,
        IDX_EXTI18 = 0x00120000,
        IDX_EXTI19 = 0x00130000,

        IDX_EXTI20 = 0x00140000,
        IDX_EXTI21 = 0x00150000,
        IDX_EXTI22 = 0x00160000,
    */

        // External Events (do not trigger execution of callbacks)
        IDX_EXTE0_PA0 = 0x10000000,
        IDX_EXTE0_PB0 = 0x10000001,
        IDX_EXTE0_PC0 = 0x10000002,
        IDX_EXTE0_PD0 = 0x10000003,
        IDX_EXTE0_PE0 = 0x10000004,
        IDX_EXTE0_PF0 = 0x10000005,
        IDX_EXTE0_PG0 = 0x10000006,
        IDX_EXTE0_PH0 = 0x10000007,
        IDX_EXTE0_PI0 = 0x10000008,

        IDX_EXTE1_PA1 = 0x10010000,
        IDX_EXTE1_PB1 = 0x10010010,
        IDX_EXTE1_PC1 = 0x10010020,
        IDX_EXTE1_PD1 = 0x10010030,
        IDX_EXTE1_PE1 = 0x10010040,
        IDX_EXTE1_PF1 = 0x10010050,
        IDX_EXTE1_PG1 = 0x10010060,
        IDX_EXTE1_PH1 = 0x10010070,
        IDX_EXTE1_PI1 = 0x10010080,

        IDX_EXTE2_PA2 = 0x10020000,
        IDX_EXTE2_PB2 = 0x10020100,
        IDX_EXTE2_PC2 = 0x10020200,
        IDX_EXTE2_PD2 = 0x10020300,
        IDX_EXTE2_PE2 = 0x10020400,
        IDX_EXTE2_PF2 = 0x10020500,
        IDX_EXTE2_PG2 = 0x10020600,
        IDX_EXTE2_PH2 = 0x10020700,
        IDX_EXTE2_PI2 = 0x10020800,

        IDX_EXTE3_PA3 = 0x10030000,
        IDX_EXTE3_PB3 = 0x10031000,
        IDX_EXTE3_PC3 = 0x10032000,
        IDX_EXTE3_PD3 = 0x10033000,
        IDX_EXTE3_PE3 = 0x10034000,
        IDX_EXTE3_PF3 = 0x10035000,
        IDX_EXTE3_PG3 = 0x10036000,
        IDX_EXTE3_PH3 = 0x10037000,
        IDX_EXTE3_PI3 = 0x10038000,


        IDX_EXTE4_PA4 = 0x10040000,
        IDX_EXTE4_PB4 = 0x10040001,
        IDX_EXTE4_PC4 = 0x10040002,
        IDX_EXTE4_PD4 = 0x10040003,
        IDX_EXTE4_PE4 = 0x10040004,
        IDX_EXTE4_PF4 = 0x10040005,
        IDX_EXTE4_PG4 = 0x10040006,
        IDX_EXTE4_PH4 = 0x10040007,
        IDX_EXTE4_PI4 = 0x10040008,

        IDX_EXTE5_PA5 = 0x10050000,
        IDX_EXTE5_PB5 = 0x10050010,
        IDX_EXTE5_PC5 = 0x10050020,
        IDX_EXTE5_PD5 = 0x10050030,
        IDX_EXTE5_PE5 = 0x10050040,
        IDX_EXTE5_PF5 = 0x10050050,
        IDX_EXTE5_PG5 = 0x10050060,
        IDX_EXTE5_PH5 = 0x10050070,
        IDX_EXTE5_PI5 = 0x10050080,

        IDX_EXTE6_PA6 = 0x10060000,
        IDX_EXTE6_PB6 = 0x10060100,
        IDX_EXTE6_PC6 = 0x10060200,
        IDX_EXTE6_PD6 = 0x10060300,
        IDX_EXTE6_PE6 = 0x10060400,
        IDX_EXTE6_PF6 = 0x10060500,
        IDX_EXTE6_PG6 = 0x10060600,
        IDX_EXTE6_PH6 = 0x10060700,
        IDX_EXTE6_PI6 = 0x10060800,

        IDX_EXTE7_PA7 = 0x10070000,
        IDX_EXTE7_PB7 = 0x10071000,
        IDX_EXTE7_PC7 = 0x10072000,
        IDX_EXTE7_PD7 = 0x10073000,
        IDX_EXTE7_PE7 = 0x10074000,
        IDX_EXTE7_PF7 = 0x10075000,
        IDX_EXTE7_PG7 = 0x10076000,
        IDX_EXTE7_PH7 = 0x10077000,
        IDX_EXTE7_PI7 = 0x10078000,

        IDX_EXTE8_PA8 = 0x10080000,
        IDX_EXTE8_PB8 = 0x10080001,
        IDX_EXTE8_PC8 = 0x10080002,
        IDX_EXTE8_PD8 = 0x10080003,
        IDX_EXTE8_PE8 = 0x10080004,
        IDX_EXTE8_PF8 = 0x10080005,
        IDX_EXTE8_PG8 = 0x10080006,
        IDX_EXTE8_PH8 = 0x10080007,
        IDX_EXTE8_PI8 = 0x10080008,
        
        IDX_EXTE9_PA9 = 0x10090000,
        IDX_EXTE9_PB9 = 0x10090010,
        IDX_EXTE9_PC9 = 0x10090020,
        IDX_EXTE9_PD9 = 0x10090030,
        IDX_EXTE9_PE9 = 0x10090040,
        IDX_EXTE9_PF9 = 0x10090050,
        IDX_EXTE9_PG9 = 0x10090060,
        IDX_EXTE9_PH9 = 0x10090070,
        IDX_EXTE9_PI9 = 0x10090080,
        
        IDX_EXTE10_PA10 = 0x100A000,
        IDX_EXTE10_PB10 = 0x100A100,
        IDX_EXTE10_PC10 = 0x100A200,
        IDX_EXTE10_PD10 = 0x100A300,
        IDX_EXTE10_PE10 = 0x100A400,
        IDX_EXTE10_PF10 = 0x100A500,
        IDX_EXTE10_PG10 = 0x100A600,
        IDX_EXTE10_PH10 = 0x100A700,
        IDX_EXTE10_PI10 = 0x100A800,
        
        IDX_EXTE11_PA11 = 0x100B0000,
        IDX_EXTE11_PB11 = 0x100B1000,
        IDX_EXTE11_PC11 = 0x100B2000,
        IDX_EXTE11_PD11 = 0x100B3000,
        IDX_EXTE11_PE11 = 0x100B4000,
        IDX_EXTE11_PF11 = 0x100B5000,
        IDX_EXTE11_PG11 = 0x100B6000,
        IDX_EXTE11_PH11 = 0x100B7000,
        IDX_EXTE11_PI11 = 0x100B8000,
        
        IDX_EXTE12_PA12 = 0x100C0000,
        IDX_EXTE12_PB12 = 0x100C0001,
        IDX_EXTE12_PC12 = 0x100C0002,
        IDX_EXTE12_PD12 = 0x100C0003,
        IDX_EXTE12_PE12 = 0x100C0004,
        IDX_EXTE12_PF12 = 0x100C0005,
        IDX_EXTE12_PG12 = 0x100C0006,
        IDX_EXTE12_PH12 = 0x100C0007,
        IDX_EXTE12_PI12 = 0x100C0008,

        IDX_EXTE13_PA13 = 0x100D0000,
        IDX_EXTE13_PB13 = 0x100D0010,
        IDX_EXTE13_PC13 = 0x100D0020,
        IDX_EXTE13_PD13 = 0x100D0030,
        IDX_EXTE13_PE13 = 0x100D0040,
        IDX_EXTE13_PF13 = 0x100D0050,
        IDX_EXTE13_PG13 = 0x100D0060,
        IDX_EXTE13_PH13 = 0x100D0070,
        IDX_EXTE13_PI13 = 0x100D0080,

        IDX_EXTE14_PA14 = 0x100E0000,
        IDX_EXTE14_PB14 = 0x100E0100,
        IDX_EXTE14_PC14 = 0x100E0200,
        IDX_EXTE14_PD14 = 0x100E0300,
        IDX_EXTE14_PE14 = 0x100E0400,
        IDX_EXTE14_PF14 = 0x100E0500,
        IDX_EXTE14_PG14 = 0x100E0600,
        IDX_EXTE14_PH14 = 0x100E0700,
        IDX_EXTE14_PI14 = 0x100E0800,

        IDX_EXTE15_PA15 = 0x100F0000,
        IDX_EXTE15_PB15 = 0x100F1000,
        IDX_EXTE15_PC15 = 0x100F2000,
        IDX_EXTE15_PD15 = 0x100F3000,
        IDX_EXTE15_PE15 = 0x100F4000,
        IDX_EXTE15_PF15 = 0x100F5000,
        IDX_EXTE15_PG15 = 0x100F6000,
        IDX_EXTE15_PH15 = 0x100F7000,
        IDX_EXTE15_PI15 = 0x100F8000,

        IDX_EXTE16 = 0x01100000,
        IDX_EXTE17 = 0x01110000,
        IDX_EXTE18 = 0x01120000,
        IDX_EXTE19 = 0x01130000,

        IDX_EXTE20 = 0x01140000,
        IDX_EXTE21 = 0x01150000,
        IDX_EXTE22 = 0x01160000,

        // having event & interrupt at the same time does not make sense, thus not added
    };

    /**
     * Rising trigger selection register (EXTI_RTSR)
     * Falling trigger selection register (EXTI_FTSR)
     * 
     * EXTI_RTSR    EXTI_FTSR
     *      0           0   Nothing
     *      0           1   falling edge triggers the interrupt
     *      1           0   rising edge triggers the interuupt
     *      1           1   both edges trigger the interrupt
     * TODO: measuring seems to yield the result that falling & rising edge are swapped!
     */
    enum class ExtIntTriggerType : DeviceSTM32F407VG::ValueTypeUnsigned 
    {
        TRIGGER_NONE                    = 0,
        TRIGGER_EDGE_FALLING            = 1,
        TRIGGER_EDGE_RISING             = 2,
        TRIGGER_EDGE_FALLING_RISING     = 3,
    };

}
}




