#ifndef HOLMES_HAL_ULTRASONIC_H
#define HOLMES_HAL_ULTRASONIC_H

#include "Gpio.h"

namespace holmes
{
  namespace internal
  {
    template<class PinType>
    class Ultrasonic
    {
    private:
      PinType _send_pin;
      PinType _receive_pin;

    public:
      Ultrasonic(PinType send_pin, PinType receive_pin) :
        _send_pin(send_pin),
        _receive_pin(receive_pin)
      {
        _send_pin.setDirection(PinType::Out);
        _receive_pin.setDirection(PinType::In);
      }

      auto measure() -> unsigned int
      {
        int time = 1;
        _send_pin.setDirection(PinType::Out);
        _send_pin.setValue(PinType::High);
        for (int i = 0; i < 100; ++i);
        _send_pin.setDirection(PinType::In);
        _send_pin.setValue(PinType::Low);
        for (int i = 0; i < 400; ++i);
        while (time < 0xFFFF && PinType::Low  == _receive_pin.getValue())
        {
          for (int i = 0; i < 10; ++i);
          ++time;
        }
        time = 1;
        while (time < 0xFFFF && PinType::High == _receive_pin.getValue())
        {
          for (int i = 0; i < 10; ++i);
          ++time;
        }
        return time;
      }
    };
  }
}

#endif // #ifndef HOLMES_HAL_ULTRASONIC_H
