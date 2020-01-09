#pragma once



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
         * @brief Each dummy class is **required** to have this public static member in order to specialize
         * the InterruptVectorTable.
         */
        static const int s_NumInterruptVectors = {26};
    };
  }
}