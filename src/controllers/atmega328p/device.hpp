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
         * @brief Inherent property of the device, which discribes the value size & type.
         */
        using ValueTypeUnsigned = uint8_t;
        using ValueTypeSigned = int8_t;
    };
  }
}