#pragma once


    
#define DISABLE(REGISTER, BIT) (REGISTER) &= ~(1 << (BIT))

#define ENABLE(REGISTER, BIT) (REGISTER) |= (1 << (BIT))

#define TOGGLE(REGISTER, BIT) (REGISTER) ^= (1 << (BIT))

#define TRIGGER(REGISTER, BIT) \
    TOGGLE(REGISTER, BIT);     \
    TOGGLE(REGISTER, BIT)

#define IS_SET(REGISTER, BIT) (REGISTER & (1 << (BIT))) != 0
