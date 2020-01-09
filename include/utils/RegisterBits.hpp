#pragma once


template <typename ResultType, ResultType Bits>
struct RegisterBits
{
    /**
     * @brief Register that is accessed at given bits
     */
    volatile ResultType* m_Register;

    /**
     * @brief Bit positions that configure the register
     *        Negative values indicate the absence of a 
     *        specific bit and all the following bits.
     *        [0,1,5,-1,-1]
     */
    int8_t m_Bits[Bits];

    /**
     * @brief Get the Masked Value object
     * 
     * @return ResultType 
     */
    ResultType getMaskedValue()
    {
        if (!m_Register)
            return 0;

        ResultType tmp = 0;

        for (ResultType i = 0; i < Bits; i++)
        {
            if (m_Bits[i] < 0)
                break;
            tmp += *m_Register & (1 << m_Bits[i]);
        }

        return tmp;
    }
};
