#pragma once

#include <avr/interrupt.h>
#include <avr/sleep.h>


namespace holmes
{
  namespace internal
  {
    class DeviceAtMega328p {
      /**
       * @brief Dummy class used for specialization
       */

      public:
        /**
         * @brief Inherent property of the device, which discribes the value size & type.
         */
        using ValueType = uint8_t;
        
        /**
         * @brief Each dummy class is **required** to have this public static member in order to specialize
         * the InterruptVectorTable.
         */
        static const ValueType s_NumInterruptVectors = {26};
    };
  }
}