#ifndef HOLMES_HAL_UART_H
#define HOLMES_HAL_UART_H

namespace holmes
{
  enum UartStatus
  {
    UartStatus_Ok,
    UartStatus_WouldBlock,
    UartStatus_NotEnoughSpace
  };

  namespace internal
  {
    template<class Impl>
    class Uart
    {
    public:

      Uart(const Uart &uart);
      Uart(Uart &&uart);

      auto operator=(const Uart &uart) -> Uart&;
      auto operator=(Uart &&uart) -> Uart&;

      auto setBaudRate(int rate) -> void;

      auto send(int length, const char *buffer, bool blocking) -> UartStatus;
      auto receive(int max, char *buffer, bool blocking) -> UartStatus;

      // TODO: callback
      // But where does the space come from to store the message that is passed
      // to the callback?
    };
  }
}

#endif // #ifndef HOLMES_HAL_UART_H
