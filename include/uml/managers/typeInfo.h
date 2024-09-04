#pragma once

#include <tuple>
#include <vector>
#include "uml/managers/baseElement.h"

namespace UML {
    // typeinfo should be TypeInfo<TList<pair<set name, set function ptr>>, TList<set BaseTypeInfos>>

    class AbstractSet;

    template <class BaseList>
    struct BaseInfo;
   
    template <class ... Bases, class Base>
    struct BaseInfo<std::tuple<Base, Bases...>> : public Base::Info, BaseInfo<std::tuple<Bases...>> {
        typedef std::tuple<Base, Bases...> BaseList;
        typedef typename Base::Info LeftBase;
        typedef BaseInfo<std::tuple<Bases...>> RightBase;
        static bool is(std::size_t elementType) {
            auto curr = LeftBase::is(elementType);
            if (curr) {
                return true;
            }

            return RightBase::is(elementType);
        }
    };

    template <class Base>
    struct BaseInfo<std::tuple<Base>> : public Base::Info {
        typedef std::tuple<Base> BaseList;
        static bool is(std::size_t elementType) {
            return elementType  == Base::template idOf<Base>();
        }
    };

    template <>
    struct BaseInfo<std::tuple<>> {
        typedef std::tuple<> BaseList;
        static bool is(__attribute__((unused)) std::size_t elementType) {
            return false;
        }
    };

    template <class ElWithSets>
    struct ElementInfo;

    template <>
    struct ElementInfo<AbstractElement> {};

    template <class BaseList = std::tuple<>, class ElementType = AbstractElement>
    struct TypeInfo : public BaseInfo<BaseList>{
        typedef ElementType Type;
        typedef ElementInfo<ElementType> Info;
    };

    typedef std::vector<std::pair<std::string, AbstractSet*>> SetList;
}
