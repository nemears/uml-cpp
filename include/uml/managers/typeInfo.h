#pragma once

#include <tuple>
#include <vector>
#include "uml/managers/baseElement.h"

namespace UML {

    class AbstractSet;

    template <class BaseList>
    struct BaseInfo;
   
    template <class ... Bases, class Base>
    struct BaseInfo<std::tuple<Base, Bases...>> : public BaseInfo<std::tuple<Bases...>> {
        typedef std::tuple<Base, Bases...> BaseList;
        static bool is(std::size_t elementType) {
            // check this base
            auto curr = Base::Info::is(elementType);
            if (curr) {
                return true;
            }

            // check other bases
            return BaseInfo<std::tuple<Bases...>>::is(elementType);
        }
    };

    template <>
    struct BaseInfo<std::tuple<>> {
        static bool is(__attribute__((unused)) std::size_t elementType) {
            return false;
        }
    };

    template <class ElWithSets>
    struct ElementInfo;

    template <>
    struct ElementInfo<AbstractElement> {};

    template <class BaseTList, class ElementType>
    struct TypeInfo : public BaseInfo<BaseTList>{
        typedef ElementType Type;
        typedef ElementInfo<ElementType> Info;
        using BaseList = BaseTList;
        static const std::size_t elementType = Type::template idOf<Type>(); // TODO doesn't work
        static bool is(std::size_t typeToCheck) {
            auto curr = typeToCheck == elementType;
            if (curr) {
                return true;
            }
            
            return BaseInfo<BaseList>::is(typeToCheck);
        }
    };

    typedef std::vector<std::pair<std::string, AbstractSet*>> SetList;
}
