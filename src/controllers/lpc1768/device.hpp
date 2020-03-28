#pragma once

#include <stdint.h>

namespace holmes
{
  namespace internal
  {
    class DeviceLPC1768 {
     
     public:
      using ValueTypeUnsigned = uint32_t;
      using ValueTypeSigned = int32_t;
    };
  }
}