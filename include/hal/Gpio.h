#ifndef HOLMES_HAL_GPIO_H
#define HOLMES_HAL_GPIO_H

namespace holmes
{
  namespace internal
  {
    template<class Impl, class Value, class Direction>
    class Gpio
    {
    public:
      static const Value Low;
      static const Value High;

      static const Direction In;
      static const Direction Out;

      Gpio(const Gpio &gpio);
      Gpio(Gpio &&gpio);

      auto operator=(const Gpio &gpio) -> Gpio&;
      auto operator=(Gpio &&gpio) -> Gpio&;

      auto getValue() -> Value;
      auto setValue(Value value) -> void;

      auto getDirection() -> Direction;
      auto setDirection(Direction direction) -> void;
    };
  }
}

#endif // #ifndef HOLMES_HAL_GPIO_H
