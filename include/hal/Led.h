#ifndef HOLMES_HAL_LED_H
#define HOLMES_HAL_LED_H

#include "Gpio.h"

namespace holmes
{
  namespace internal
  {
    template<class PinType>
    class Led
    {
    private:
      PinType _pin;

    public:
      Led(PinType pin) : _pin(pin)
      {
        _pin.setValue(PinType::Low);
        _pin.setDirection(PinType::Out);
      }

      auto on() -> void
      {
        _pin.setValue(PinType::High);
      }
      auto off() -> void
      {
        _pin.setValue(PinType::Low);
      }
    };
  }
}

#endif // #ifdef HOLMES_HAL_LED_H
