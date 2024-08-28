#pragma once

#include <tuple>

namespace UML {
    // typeinfo should be TypeInfo<TList<pair<set name, set function ptr>>, TList<set BaseTypeInfos>>

    template <class BaseList>
    class TypeInfo;
    
    template <class ... Bases, class Base>
    class TypeInfo<std::tuple<Base, Bases...>> : public Base::Info, TypeInfo<std::tuple<Bases...>> {
        typedef std::tuple<Base, Bases...> BaseList;
        typedef typename Base::Info LeftBase;
        typedef TypeInfo<std::tuple<Bases...>> RightBase;
        template <class T>
        constexpr bool is() {
            auto curr = LeftBase::template is<T>();
            if (curr) {
                return true;
            }

            return RightBase::template is<T>();
        }
    };

    template <class Base>
    class TypeInfo<std::tuple<Base>> : public Base::Info {
        typedef typename Base::Info LeftBase;
        const std::size_t elementType = Base::elementType;
        template <class T>
        constexpr bool is() {
            return Base::template FindIdOf<T>::value  == Base::template FindIdOf<Base>;
        }
    };

    template <>
    class TypeInfo<std::tuple<>> {
        constexpr bool is() {
            return false;
        }
    };
}
