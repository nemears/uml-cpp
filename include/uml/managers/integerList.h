#pragma once

#include <cstddef>

namespace UML {
    // compile time list of integers
    template<std::size_t ... Is>
    struct IntList {};

    // has integer in list
    template <std::size_t I, typename Ints>
    struct HasInt;

    template <std::size_t I, std::size_t J, std::size_t... Is>
    struct HasInt<I, IntList<J, Is...>> {
        static const bool value = I == J ? true : HasInt<I, IntList<Is...>>::value;
    };

    template<std::size_t I>
    struct HasInt<I, IntList<>> {
        static const bool value = false;
    };

    // add integer to list
    template <class, std::size_t>
    struct IntAppend;

    template <std::size_t ... Is, std::size_t Right>
    struct IntAppend<IntList<Is...>, Right> {
        using type = IntList<Is..., Right>;
    };
}
