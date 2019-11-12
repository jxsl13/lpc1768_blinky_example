template <ValueType Bits>
struct RegisterBits
{
    /**
     * @brief Register that is accessed at given bits
     */
    volatile ValueType* m_Register;

    /**
     * @brief Bit positions that configure the register
     *        Negative values indicate the absence of a 
     *        specific bit and all the following bits.
     *        [0,1,5,-1,-1]
     */
    int8_t m_Bits[Bits];

    ValueType getMaskedValue()
    {
        if (!m_Register)
            return 0;

        ValueType tmp = 0;

        for (ValueType i = 0; i < Bits; i++)
        {
            if (m_Bits[i] < 0)
                break;
            tmp += *m_Register & (1 << m_Bits[i]);
        }

        return tmp;
    }
};