#ifndef HOLMES_HAL_TIMER_H
#define HOLMES_HAL_TIMER_H

namespace holmes
{
  namespace internal
  {
    template<class Impl, class TimeType>
    class Timer
    {
    public:

      Timer(const Timer &timer);
      Timer(Timer &&timer);

      auto operator=(const Timer &timer) -> Timer&;
      auto operator=(Timer &&timer) -> Timer&;

      auto setTimeout(TimeType time) -> void;
      auto getTimeout() -> TimeType;

      auto addCallback(void (*func)()) -> void;
    };
  }
}

#endif // #ifndef HOLMES_HAL_TIMER_H
