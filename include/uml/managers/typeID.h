#pragma once

#include <tuple>

namespace UML {
    // below is implementation of TypeIDHierarchy from Alex Andrescu's C++ Generic Programming and Design Patterns Applied
    // with variardic templates from c++11

    // type id
    template <std::size_t ElementType, class Tlist, template<std::size_t, class> class Unit>
    class TypeIDHierarchy;

    template <std::size_t ElementType, class ... Ts, class T, template<std::size_t, class> class Unit>
    class TypeIDHierarchy<ElementType, std::tuple<T, Ts...>, Unit> : public TypeIDHierarchy<ElementType, T, Unit>, public TypeIDHierarchy<ElementType - 1, std::tuple<Ts...>, Unit> {
        public:
            typedef std::tuple<T, Ts...> Tlist;
            typedef TypeIDHierarchy<ElementType, T, Unit> LeftBase;
            typedef TypeIDHierarchy<ElementType - 1, std::tuple<Ts...>, Unit> RightBase;
    };

    template <std::size_t ElementType, class AtomicType, template <std::size_t, class> class Unit>
    class TypeIDHierarchy: public Unit<ElementType, AtomicType> {
        typedef Unit<ElementType, AtomicType> LeftBase;
    };

    template <std::size_t ElementType, template <std::size_t, class> class Unit> 
    class TypeIDHierarchy<ElementType, std::tuple<>, Unit> {};

    template <std::size_t ElementType, class T>
    struct TypeID {
        static std::size_t elementType() {
            return ElementType;
        }
    };

    template <class Tlist>
    class ManagerTypes : public TypeIDHierarchy<std::tuple_size<Tlist>{}, Tlist, TypeID> {
        typedef Tlist Types;

        // utility class to get type given size_t
        template <std::size_t ElementType> 
        struct GetType {
            using type = std::tuple_element<ElementType, Types>;
        };

        // utility class to get elementType given type
        template<class TTlist, class T>
        struct TypeIdOfHelper;

        template <class T>
        struct TypeIdOfHelper<std::tuple<>, T> {
            enum { value = -1 };
        };

        template <class T, class Tail> 
        struct TypeIdOfHelper<std::tuple<T>, Tail> {
            enum { value = 0 };
        };

        template <class ... TTs, class Head, class T>
        struct TypeIdOfHelper<std::tuple<Head, TTs...>, T> {
            private:
                enum { temp = TypeIdOfHelper<TTs...>::value };
            public:
                enum { value = temp == -1 ? -1 : 1 + temp }; 
        };
        
        template <class T>
        struct TypeIdOf : public TypeIdOfHelper<Types, T> {};
    };
}
