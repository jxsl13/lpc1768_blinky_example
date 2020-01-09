#ifndef HOLMES_HAL_PWM_H
#define HOLMES_HAL_PWM_H

namespace holmes
{
  namespace internal
  {
    template<class Impl, class TimeType>
    class Pwm
    {
    public:

      Pwm(const Pwm &pwm);
      Pwm(Pwm &&pwm);

      auto operator=(const Pwm &pwm) -> Pwm&;
      auto operator=(Pwm &&pwm) -> Pwm&;

      auto getPeriod() -> TimeType;
      auto setPeriod(TimeType period) -> void;

      auto getPulse() -> TimeType;
      auto setPulse(TimeType pulse) -> void;

      auto enable() -> void;
      auto disable() -> void;
    };
  }
}

#endif // #ifndef HOLMES_HAL_PWM_H
