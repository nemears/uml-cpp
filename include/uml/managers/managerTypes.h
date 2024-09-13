#pragma once

#include <tuple>

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
            
        template <class T>
        static bool is(std::size_t elementType) {
            return T::Info::is(elementType);
        }
    };
}
