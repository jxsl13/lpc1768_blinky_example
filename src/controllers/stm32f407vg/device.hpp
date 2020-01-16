#pragma once
#include "stm32f4xx.h"



namespace holmes
{
  namespace internal
  {
    class DeviceSTM32F407VG {
      /**
       * @brief Dummy class used for specialization
       */

      public:
        /**
         * @brief Each dummy class is **required** to have this public static member in order to specialize
         * the InterruptVectorTable.
         */
        static const int s_NumInterruptVectors = {82 + 16};  // 82 mutable + 16 immutable vectors};
    };
  }
}