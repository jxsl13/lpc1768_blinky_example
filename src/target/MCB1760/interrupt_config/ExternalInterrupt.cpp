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
    
    
}

void ExternalInterrupt::retrieveFrom(ExternalInterrupt::IndexType InterruptIndex)
{
    ValueType Index = static_cast<ValueType>(InterruptIndex);

}


#ifdef __cplusplus
}
#endif