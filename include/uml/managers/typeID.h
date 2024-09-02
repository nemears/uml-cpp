#pragma once

#include <tuple>

namespace UML {
    // // below is implementation of TypeIDHierarchy from Alex Andrescu's C++ Generic Programming and Design Patterns Applied
    // // with variardic templates from c++11

    // // type id
    // template <class Tlist, template<std::size_t, class> class Unit>
    // struct TypeIDHierarchy;

    // template <class ... Ts, class T, template<std::size_t, class> class Unit>
    // struct TypeIDHierarchy<std::tuple<T, Ts...>, Unit> : public TypeIDHierarchy<std::tuple<T>, Unit>, public TypeIDHierarchy<std::tuple<Ts...>, Unit> {
    //         typedef std::tuple<T, Ts...> Tlist;
    //         typedef Unit<std::tuple_size<std::tuple<T, Ts...>>{}, T> Front;
    // };

    // template <class AtomicType, template <std::size_t, class> class Unit>
    // struct TypeIDHierarchy<std::tuple<AtomicType>, Unit> : public Unit<1 , AtomicType> {
    //     typedef Unit<ElementType, AtomicType> FrontID;
    // };

    // template <template <std::size_t, class> class Unit> 
    // struct TypeIDHierarchy<ElementType, std::tuple<>, Unit> {};

    // template <class Tlist, class T>
    // struct TypeID {
    //     static std::size_t elementType() {
    //         return ElementType;
    //     }
    // };
    
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
        static std::size_t idOf() {
            return Index<T, Tlist>::value;
        }
            
        // // utility class to get elementType given type
        // template<class TTlist, class T>
        // struct TypeIdOfHelper;

        // template <class T>
        // struct TypeIdOfHelper<std::tuple<>, T> {
        //     enum { value = -1 };
        // };

        // template <class T, class Tail> 
        // struct TypeIdOfHelper<std::tuple<T>, Tail> {
        //     enum { value = 0 };
        // };

        // template <class ... TTs, class Head, class T>
        // struct TypeIdOfHelper<std::tuple<Head, TTs...>, T> {
        //     private:
        //         enum { temp = TypeIdOfHelper<std::tuple<TTs...>, T>::value };
        //     public:
        //         enum { value = temp == -1 ? -1 : 1 + temp }; 
        // };
        // 
        // template <class T>
        // struct TypeIdOf : public TypeIdOfHelper<Types, T> {};

        template <class T>
        static bool is(std::size_t elementType) {
            return T::Info::is(elementType);
        }
    };
}
