#pragma once

#include <tuple>
#include "uml/managers/abstractManager.h"

namespace UML {
    
    template <class T, class Tuple>
    struct Index;

    template <class T, class... Types>
    struct Index<T, std::tuple<T, Types...>> {
        static const std::size_t value = 0;
    };

    template <class T, class U, class... Types>
    struct Index<T, std::tuple<U, Types...>> {
        static const std::size_t value = 1 + Index<T, std::tuple<Types...>>::value;
    };

    template <class Tlist>
    struct ManagerTypes {
        typedef Tlist Types;

        template <class T>
        static constexpr std::size_t idOf() {
            return Index<T, Tlist>::value;
        }
            
        template <class T, std::size_t I = 0>
        static bool is(std::size_t elementType) {
            if constexpr (std::tuple_size<Types>{} > I) {
                if (I == elementType) {
                    return std::tuple_element_t<I, Types>::Info::is(T::Info::elementType);
                }
                return is<T, I +1>(elementType);
            }
            throw ManagerStateException("Bad Type given to is!");
        }
    };
}
