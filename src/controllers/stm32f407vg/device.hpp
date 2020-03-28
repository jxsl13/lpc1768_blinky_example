#pragma once

#include <stdint.h>

namespace holmes
{
  namespace internal
  {
    class DeviceSTM32F407VG {
      public:
        using ValueTypeUnsigned = uint32_t;
        using ValueTypeSigned = int32_t;
    };
  }
}