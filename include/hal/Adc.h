#ifndef HOLMES_HAL_ADC_H
#define HOLMES_HAL_ADC_H

namespace holmes
{
  namespace internal
  {
    template<class Impl, class ResultType>
    class Adc
    {
    public:

      Adc(const Adc &adc);
      Adc(Adc &&adc);

      auto operator=(const Adc &adc) -> Adc&;
      auto operator=(Adc &&adc) -> Adc&;

      auto getMax() -> ResultType;

      auto measure() -> ResultType;
    };
  }
}

#endif // #ifndef HOLMES_HAL_ADC_H
