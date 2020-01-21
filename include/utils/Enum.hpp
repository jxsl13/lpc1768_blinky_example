#pragma once


// https://stackoverflow.com/questions/4969233/how-to-check-if-enum-value-is-valid

template<typename EnumType, EnumType... Values> class EnumCheck;

template<typename EnumType> class EnumCheck<EnumType>
{
public:
    template<typename IntType>
    static bool constexpr isValue(IntType) { return false; }
};

template<typename EnumType, EnumType V, EnumType... Next>
class EnumCheck<EnumType, V, Next...> : private EnumCheck<EnumType, Next...>
{
    using super = EnumCheck<EnumType, Next...>;

public:
    template<typename IntType>
    static bool constexpr isValue(IntType v)
    {
        return v == static_cast<IntType>(V) || super::isValue(v);
    }
};


/*
    enum class Test {
        A = 1,
        C = 3,
        E = 5
    };

    using TestCheck = EnumCheck<Test, Test::A, Test::C, Test::E>;

    void check_value(int v)
    {
        if (TestCheck::is_value(v))
            printf("%d is OK\n", v);
        else
            printf("%d is not OK\n", v);
    }

    int main()
    {
        for (int i = 0; i < 10; ++i)
            check_value(i);
    }
*/