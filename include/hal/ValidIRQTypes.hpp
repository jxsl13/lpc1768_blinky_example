#pragma once

struct ValidIRQTypes
{
     const ValueType m_size;
     const ValueType m_ValidTypes[ValidInterruptCount];

    inline ValueType size() { return m_size; };

     /**
      * @brief Binary search to check if the given index is valid.
      * 
      * @param InterruptNumber 
      * @return true 
      * @return false 
      */
    bool IsValidIRQType(const ValueType InterruptNumber) const
    {
         /**
          * @brief Binary search
          */
        ValueType i = 0;
        ValueType j = m_size - 1;

        if (InterruptNumber == m_ValidTypes[i] || InterruptNumber == m_ValidTypes[j])
            return true;
        else if (InterruptNumber < m_ValidTypes[i] || m_ValidTypes[j] < InterruptNumber)
            return false;

        ValueType mid = 0;
        ValueType midValue = 0;
        while (i < j)
        {
            mid = i + (j - i) / 2;
            midValue = m_ValidTypes[mid];

            if (InterruptNumber <= midValue)
                j = mid - 1;
            else
                i = mid + 1;
        }

        return InterruptNumber == midValue;
    }
};
