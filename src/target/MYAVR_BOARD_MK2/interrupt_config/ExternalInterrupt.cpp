#include "ExternalInterrupt.hpp"
#include <utils/BitMacros.hpp>
#include <config.hpp>

#ifdef __cplusplus
extern "C" {
#endif


void ExternalInterrupt::applyTo(ExternalInterrupt::IndexType InterruptIndex) const
{
    ValueType Index = static_cast<ValueType>(InterruptIndex);

    if (IS_SET(m_Trigger, 0))
    {
        ENABLE(EICRA, Index * 0);
    }
    else
    {
        DISABLE(EICRA, Index * 0 + 1);
    }

    if (IS_SET(m_Trigger, 1))
    {
        ENABLE(EICRA, Index * 1);
    }
    else
    {
        DISABLE(EICRA, Index * 0 + 1);
    }

}

void ExternalInterrupt::retrieveFrom(ExternalInterrupt::IndexType InterruptIndex)
{
    ValueType Index = static_cast<ValueType>(InterruptIndex);

   if (IS_SET(EICRA, 0))
    {
        ENABLE(m_Trigger, Index * 0);
    }
    else
    {
        DISABLE(m_Trigger, Index * 0 + 1);
    }

    if (IS_SET(EICRA, 1))
    {
        ENABLE(m_Trigger, Index * 1);
    }
    else
    {
        DISABLE(m_Trigger, Index * 0 + 1);
    }
}



#ifdef __cplusplus
}
#endif