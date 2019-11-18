#include "ExternalInterrupt.hpp"
#include <utils/BitMacros.hpp>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Number of external interrupts
 */
constexpr ValueType size = 4;

void ExternalInterrupt::applyTo(ExternalInterrupt::IndexType InterruptIndex) const
{
    ValueType Index = static_cast<ValueType>(InterruptIndex);
    //TODO: Implement this
    
}

void ExternalInterrupt::retrieveFrom(ExternalInterrupt::IndexType InterruptIndex)
{
    ValueType Index = static_cast<ValueType>(InterruptIndex);
    //TODO: Implement this
}


#ifdef __cplusplus
}
#endif