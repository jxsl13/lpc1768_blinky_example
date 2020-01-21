#pragma once
#include "stm32f4xx.h" // TODO: move to using cpp/hpp



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
        // TODO: remove from dummy class, as it's only used for interrupts and nowhere else
        static const int s_NumInterruptVectors = {82 + 16};  // 82 mutable + 16 immutable vectors};
    };
  }
}