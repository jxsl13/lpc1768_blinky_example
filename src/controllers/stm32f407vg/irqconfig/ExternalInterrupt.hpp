#pragma once

#include <hal/irqconfig/ExternalInterrupt.hpp>
#include "../device.hpp"

namespace holmes
{
namespace internal
{

    enum class ExtIntConfigType : DeviceSTM32F407VG::ValueTypeUnsigned;
    enum class ExtIntTriggerType : DeviceSTM32F407VG::ValueTypeUnsigned;

    template<>
    class ExternalInterrupt<DeviceSTM32F407VG, ExtIntConfigType, ExtIntTriggerType>
    {
        // only used internally
        using ValueType =   DeviceSTM32F407VG::ValueTypeUnsigned;
    public:
        using ConfigType =   ExtIntConfigType;
        using TriggerType = ExtIntTriggerType;
        using CallbackType = void (*)(void);

        ExternalInterrupt(ConfigType InterruptIndex, TriggerType Trigger);
        auto applyTo(ConfigType InterruptIndex) const -> void;
        auto apply() const -> void;
        static auto clearPendingBitOf(ConfigType InterruptIndex) -> void;
        auto clearPendingBit() -> void;
    
    private:
        ConfigType m_Config;
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
    enum class ExtIntConfigType : DeviceSTM32F407VG::ValueTypeUnsigned {
    
        // External Interrupts
        CFG_EXTI0_PA0 = 0x01000000,
        CFG_EXTI0_PB0 = 0x01000001,
        CFG_EXTI0_PC0 = 0x01000002,
        CFG_EXTI0_PD0 = 0x01000003,
        CFG_EXTI0_PE0 = 0x01000004,
        CFG_EXTI0_PF0 = 0x01000005,
        CFG_EXTI0_PG0 = 0x01000006,
        CFG_EXTI0_PH0 = 0x01000007,
        CFG_EXTI0_PI0 = 0x01000008,

        CFG_EXTI1_PA1 = 0x01010000,
        CFG_EXTI1_PB1 = 0x01010010,
        CFG_EXTI1_PC1 = 0x01010020,
        CFG_EXTI1_PD1 = 0x01010030,
        CFG_EXTI1_PE1 = 0x01010040,
        CFG_EXTI1_PF1 = 0x01010050,
        CFG_EXTI1_PG1 = 0x01010060,
        CFG_EXTI1_PH1 = 0x01010070,
        CFG_EXTI1_PI1 = 0x01010080,

        CFG_EXTI2_PA2 = 0x01020000,
        CFG_EXTI2_PB2 = 0x01020100,
        CFG_EXTI2_PC2 = 0x01020200,
        CFG_EXTI2_PD2 = 0x01020300,
        CFG_EXTI2_PE2 = 0x01020400,
        CFG_EXTI2_PF2 = 0x01020500,
        CFG_EXTI2_PG2 = 0x01020600,
        CFG_EXTI2_PH2 = 0x01020700,
        CFG_EXTI2_PI2 = 0x01020800,

        CFG_EXTI3_PA3 = 0x01030000,
        CFG_EXTI3_PB3 = 0x01031000,
        CFG_EXTI3_PC3 = 0x01032000,
        CFG_EXTI3_PD3 = 0x01033000,
        CFG_EXTI3_PE3 = 0x01034000,
        CFG_EXTI3_PF3 = 0x01035000,
        CFG_EXTI3_PG3 = 0x01036000,
        CFG_EXTI3_PH3 = 0x01037000,
        CFG_EXTI3_PI3 = 0x01038000,


        CFG_EXTI4_PA4 = 0x01040000,
        CFG_EXTI4_PB4 = 0x01040001,
        CFG_EXTI4_PC4 = 0x01040002,
        CFG_EXTI4_PD4 = 0x01040003,
        CFG_EXTI4_PE4 = 0x01040004,
        CFG_EXTI4_PF4 = 0x01040005,
        CFG_EXTI4_PG4 = 0x01040006,
        CFG_EXTI4_PH4 = 0x01040007,
        CFG_EXTI4_PI4 = 0x01040008,

        CFG_EXTI5_PA5 = 0x01050000,
        CFG_EXTI5_PB5 = 0x01050010,
        CFG_EXTI5_PC5 = 0x01050020,
        CFG_EXTI5_PD5 = 0x01050030,
        CFG_EXTI5_PE5 = 0x01050040,
        CFG_EXTI5_PF5 = 0x01050050,
        CFG_EXTI5_PG5 = 0x01050060,
        CFG_EXTI5_PH5 = 0x01050070,
        CFG_EXTI5_PI5 = 0x01050080,

        CFG_EXTI6_PA6 = 0x01060000,
        CFG_EXTI6_PB6 = 0x01060100,
        CFG_EXTI6_PC6 = 0x01060200,
        CFG_EXTI6_PD6 = 0x01060300,
        CFG_EXTI6_PE6 = 0x01060400,
        CFG_EXTI6_PF6 = 0x01060500,
        CFG_EXTI6_PG6 = 0x01060600,
        CFG_EXTI6_PH6 = 0x01060700,
        CFG_EXTI6_PI6 = 0x01060800,

        CFG_EXTI7_PA7 = 0x01070000,
        CFG_EXTI7_PB7 = 0x01071000,
        CFG_EXTI7_PC7 = 0x01072000,
        CFG_EXTI7_PD7 = 0x01073000,
        CFG_EXTI7_PE7 = 0x01074000,
        CFG_EXTI7_PF7 = 0x01075000,
        CFG_EXTI7_PG7 = 0x01076000,
        CFG_EXTI7_PH7 = 0x01077000,
        CFG_EXTI7_PI7 = 0x01078000,

        CFG_EXTI8_PA8 = 0x01080000,
        CFG_EXTI8_PB8 = 0x01080001,
        CFG_EXTI8_PC8 = 0x01080002,
        CFG_EXTI8_PD8 = 0x01080003,
        CFG_EXTI8_PE8 = 0x01080004,
        CFG_EXTI8_PF8 = 0x01080005,
        CFG_EXTI8_PG8 = 0x01080006,
        CFG_EXTI8_PH8 = 0x01080007,
        CFG_EXTI8_PI8 = 0x01080008,
        
        CFG_EXTI9_PA9 = 0x01090000,
        CFG_EXTI9_PB9 = 0x01090010,
        CFG_EXTI9_PC9 = 0x01090020,
        CFG_EXTI9_PD9 = 0x01090030,
        CFG_EXTI9_PE9 = 0x01090040,
        CFG_EXTI9_PF9 = 0x01090050,
        CFG_EXTI9_PG9 = 0x01090060,
        CFG_EXTI9_PH9 = 0x01090070,
        CFG_EXTI9_PI9 = 0x01090080,
        
        CFG_EXTI10_PA10 = 0x010A0000,
        CFG_EXTI10_PB10 = 0x010A0100,
        CFG_EXTI10_PC10 = 0x010A0200,
        CFG_EXTI10_PD10 = 0x010A0300,
        CFG_EXTI10_PE10 = 0x010A0400,
        CFG_EXTI10_PF10 = 0x010A0500,
        CFG_EXTI10_PG10 = 0x010A0600,
        CFG_EXTI10_PH10 = 0x010A0700,
        CFG_EXTI10_PI10 = 0x010A0800,
        
        CFG_EXTI11_PA11 = 0x010B0000,
        CFG_EXTI11_PB11 = 0x010B1000,
        CFG_EXTI11_PC11 = 0x010B2000,
        CFG_EXTI11_PD11 = 0x010B3000,
        CFG_EXTI11_PE11 = 0x010B4000,
        CFG_EXTI11_PF11 = 0x010B5000,
        CFG_EXTI11_PG11 = 0x010B6000,
        CFG_EXTI11_PH11 = 0x010B7000,
        CFG_EXTI11_PI11 = 0x010B8000,
        
        CFG_EXTI12_PA12 = 0x010C0000,
        CFG_EXTI12_PB12 = 0x010C0001,
        CFG_EXTI12_PC12 = 0x010C0002,
        CFG_EXTI12_PD12 = 0x010C0003,
        CFG_EXTI12_PE12 = 0x010C0004,
        CFG_EXTI12_PF12 = 0x010C0005,
        CFG_EXTI12_PG12 = 0x010C0006,
        CFG_EXTI12_PH12 = 0x010C0007,
        CFG_EXTI12_PI12 = 0x010C0008,

        CFG_EXTI13_PA13 = 0x010D0000,
        CFG_EXTI13_PB13 = 0x010D0010,
        CFG_EXTI13_PC13 = 0x010D0020,
        CFG_EXTI13_PD13 = 0x010D0030,
        CFG_EXTI13_PE13 = 0x010D0040,
        CFG_EXTI13_PF13 = 0x010D0050,
        CFG_EXTI13_PG13 = 0x010D0060,
        CFG_EXTI13_PH13 = 0x010D0070,
        CFG_EXTI13_PI13 = 0x010D0080,

        CFG_EXTI14_PA14 = 0x010E0000,
        CFG_EXTI14_PB14 = 0x010E0100,
        CFG_EXTI14_PC14 = 0x010E0200,
        CFG_EXTI14_PD14 = 0x010E0300,
        CFG_EXTI14_PE14 = 0x010E0400,
        CFG_EXTI14_PF14 = 0x010E0500,
        CFG_EXTI14_PG14 = 0x010E0600,
        CFG_EXTI14_PH14 = 0x010E0700,
        CFG_EXTI14_PI14 = 0x010E0800,

        CFG_EXTI15_PA15 = 0x010F0000,
        CFG_EXTI15_PB15 = 0x010F1000,
        CFG_EXTI15_PC15 = 0x010F2000,
        CFG_EXTI15_PD15 = 0x010F3000,
        CFG_EXTI15_PE15 = 0x010F4000,
        CFG_EXTI15_PF15 = 0x010F5000,
        CFG_EXTI15_PG15 = 0x010F6000,
        CFG_EXTI15_PH15 = 0x010F7000,
        CFG_EXTI15_PI15 = 0x010F8000,

        CFG_EXTI16      = 0x01100000,
        CFG_EXTI17      = 0x01110000,
        CFG_EXTI18      = 0x01120000,
        CFG_EXTI19      = 0x01130000,
        CFG_EXTI20      = 0x01140000,
        CFG_EXTI21      = 0x01150000,
        CFG_EXTI22      = 0x01160000,

        // External Events (do not trigger execution of callbacks)
        CFG_EXTE0_PA0 = 0x10000000,
        CFG_EXTE0_PB0 = 0x10000001,
        CFG_EXTE0_PC0 = 0x10000002,
        CFG_EXTE0_PD0 = 0x10000003,
        CFG_EXTE0_PE0 = 0x10000004,
        CFG_EXTE0_PF0 = 0x10000005,
        CFG_EXTE0_PG0 = 0x10000006,
        CFG_EXTE0_PH0 = 0x10000007,
        CFG_EXTE0_PI0 = 0x10000008,

        CFG_EXTE1_PA1 = 0x10010000,
        CFG_EXTE1_PB1 = 0x10010010,
        CFG_EXTE1_PC1 = 0x10010020,
        CFG_EXTE1_PD1 = 0x10010030,
        CFG_EXTE1_PE1 = 0x10010040,
        CFG_EXTE1_PF1 = 0x10010050,
        CFG_EXTE1_PG1 = 0x10010060,
        CFG_EXTE1_PH1 = 0x10010070,
        CFG_EXTE1_PI1 = 0x10010080,

        CFG_EXTE2_PA2 = 0x10020000,
        CFG_EXTE2_PB2 = 0x10020100,
        CFG_EXTE2_PC2 = 0x10020200,
        CFG_EXTE2_PD2 = 0x10020300,
        CFG_EXTE2_PE2 = 0x10020400,
        CFG_EXTE2_PF2 = 0x10020500,
        CFG_EXTE2_PG2 = 0x10020600,
        CFG_EXTE2_PH2 = 0x10020700,
        CFG_EXTE2_PI2 = 0x10020800,

        CFG_EXTE3_PA3 = 0x10030000,
        CFG_EXTE3_PB3 = 0x10031000,
        CFG_EXTE3_PC3 = 0x10032000,
        CFG_EXTE3_PD3 = 0x10033000,
        CFG_EXTE3_PE3 = 0x10034000,
        CFG_EXTE3_PF3 = 0x10035000,
        CFG_EXTE3_PG3 = 0x10036000,
        CFG_EXTE3_PH3 = 0x10037000,
        CFG_EXTE3_PI3 = 0x10038000,


        CFG_EXTE4_PA4 = 0x10040000,
        CFG_EXTE4_PB4 = 0x10040001,
        CFG_EXTE4_PC4 = 0x10040002,
        CFG_EXTE4_PD4 = 0x10040003,
        CFG_EXTE4_PE4 = 0x10040004,
        CFG_EXTE4_PF4 = 0x10040005,
        CFG_EXTE4_PG4 = 0x10040006,
        CFG_EXTE4_PH4 = 0x10040007,
        CFG_EXTE4_PI4 = 0x10040008,

        CFG_EXTE5_PA5 = 0x10050000,
        CFG_EXTE5_PB5 = 0x10050010,
        CFG_EXTE5_PC5 = 0x10050020,
        CFG_EXTE5_PD5 = 0x10050030,
        CFG_EXTE5_PE5 = 0x10050040,
        CFG_EXTE5_PF5 = 0x10050050,
        CFG_EXTE5_PG5 = 0x10050060,
        CFG_EXTE5_PH5 = 0x10050070,
        CFG_EXTE5_PI5 = 0x10050080,

        CFG_EXTE6_PA6 = 0x10060000,
        CFG_EXTE6_PB6 = 0x10060100,
        CFG_EXTE6_PC6 = 0x10060200,
        CFG_EXTE6_PD6 = 0x10060300,
        CFG_EXTE6_PE6 = 0x10060400,
        CFG_EXTE6_PF6 = 0x10060500,
        CFG_EXTE6_PG6 = 0x10060600,
        CFG_EXTE6_PH6 = 0x10060700,
        CFG_EXTE6_PI6 = 0x10060800,

        CFG_EXTE7_PA7 = 0x10070000,
        CFG_EXTE7_PB7 = 0x10071000,
        CFG_EXTE7_PC7 = 0x10072000,
        CFG_EXTE7_PD7 = 0x10073000,
        CFG_EXTE7_PE7 = 0x10074000,
        CFG_EXTE7_PF7 = 0x10075000,
        CFG_EXTE7_PG7 = 0x10076000,
        CFG_EXTE7_PH7 = 0x10077000,
        CFG_EXTE7_PI7 = 0x10078000,

        CFG_EXTE8_PA8 = 0x10080000,
        CFG_EXTE8_PB8 = 0x10080001,
        CFG_EXTE8_PC8 = 0x10080002,
        CFG_EXTE8_PD8 = 0x10080003,
        CFG_EXTE8_PE8 = 0x10080004,
        CFG_EXTE8_PF8 = 0x10080005,
        CFG_EXTE8_PG8 = 0x10080006,
        CFG_EXTE8_PH8 = 0x10080007,
        CFG_EXTE8_PI8 = 0x10080008,
        
        CFG_EXTE9_PA9 = 0x10090000,
        CFG_EXTE9_PB9 = 0x10090010,
        CFG_EXTE9_PC9 = 0x10090020,
        CFG_EXTE9_PD9 = 0x10090030,
        CFG_EXTE9_PE9 = 0x10090040,
        CFG_EXTE9_PF9 = 0x10090050,
        CFG_EXTE9_PG9 = 0x10090060,
        CFG_EXTE9_PH9 = 0x10090070,
        CFG_EXTE9_PI9 = 0x10090080,
        
        CFG_EXTE10_PA10 = 0x100A000,
        CFG_EXTE10_PB10 = 0x100A100,
        CFG_EXTE10_PC10 = 0x100A200,
        CFG_EXTE10_PD10 = 0x100A300,
        CFG_EXTE10_PE10 = 0x100A400,
        CFG_EXTE10_PF10 = 0x100A500,
        CFG_EXTE10_PG10 = 0x100A600,
        CFG_EXTE10_PH10 = 0x100A700,
        CFG_EXTE10_PI10 = 0x100A800,
        
        CFG_EXTE11_PA11 = 0x100B0000,
        CFG_EXTE11_PB11 = 0x100B1000,
        CFG_EXTE11_PC11 = 0x100B2000,
        CFG_EXTE11_PD11 = 0x100B3000,
        CFG_EXTE11_PE11 = 0x100B4000,
        CFG_EXTE11_PF11 = 0x100B5000,
        CFG_EXTE11_PG11 = 0x100B6000,
        CFG_EXTE11_PH11 = 0x100B7000,
        CFG_EXTE11_PI11 = 0x100B8000,
        
        CFG_EXTE12_PA12 = 0x100C0000,
        CFG_EXTE12_PB12 = 0x100C0001,
        CFG_EXTE12_PC12 = 0x100C0002,
        CFG_EXTE12_PD12 = 0x100C0003,
        CFG_EXTE12_PE12 = 0x100C0004,
        CFG_EXTE12_PF12 = 0x100C0005,
        CFG_EXTE12_PG12 = 0x100C0006,
        CFG_EXTE12_PH12 = 0x100C0007,
        CFG_EXTE12_PI12 = 0x100C0008,

        CFG_EXTE13_PA13 = 0x100D0000,
        CFG_EXTE13_PB13 = 0x100D0010,
        CFG_EXTE13_PC13 = 0x100D0020,
        CFG_EXTE13_PD13 = 0x100D0030,
        CFG_EXTE13_PE13 = 0x100D0040,
        CFG_EXTE13_PF13 = 0x100D0050,
        CFG_EXTE13_PG13 = 0x100D0060,
        CFG_EXTE13_PH13 = 0x100D0070,
        CFG_EXTE13_PI13 = 0x100D0080,

        CFG_EXTE14_PA14 = 0x100E0000,
        CFG_EXTE14_PB14 = 0x100E0100,
        CFG_EXTE14_PC14 = 0x100E0200,
        CFG_EXTE14_PD14 = 0x100E0300,
        CFG_EXTE14_PE14 = 0x100E0400,
        CFG_EXTE14_PF14 = 0x100E0500,
        CFG_EXTE14_PG14 = 0x100E0600,
        CFG_EXTE14_PH14 = 0x100E0700,
        CFG_EXTE14_PI14 = 0x100E0800,

        CFG_EXTE15_PA15 = 0x100F0000,
        CFG_EXTE15_PB15 = 0x100F1000,
        CFG_EXTE15_PC15 = 0x100F2000,
        CFG_EXTE15_PD15 = 0x100F3000,
        CFG_EXTE15_PE15 = 0x100F4000,
        CFG_EXTE15_PF15 = 0x100F5000,
        CFG_EXTE15_PG15 = 0x100F6000,
        CFG_EXTE15_PH15 = 0x100F7000,
        CFG_EXTE15_PI15 = 0x100F8000,

        CFG_EXTE16      = 0x10100000,
        CFG_EXTE17      = 0x10110000,
        CFG_EXTE18      = 0x10120000,
        CFG_EXTE19      = 0x10130000,
        CFG_EXTE20      = 0x10140000,
        CFG_EXTE21      = 0x10150000,
        CFG_EXTE22      = 0x10160000,

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




