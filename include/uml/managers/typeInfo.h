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

    typedef std::vector<std::pair<std::string, AbstractSet*>> SetList;

    struct DefaultInfo {
        static const bool abstract = true;
        static const bool extraData = false;
        static SetList sets(__attribute__((unused)) AbstractElement& el) {
            return SetList{};
        }
    };

    template <class ElWithSets>
    struct ElementInfo;

    template <>
    struct ElementInfo<AbstractElement> : public DefaultInfo {};

    template <class BaseTList, class ElementType>
    struct TypeInfo : public ElementInfo<ElementType>, public BaseInfo<BaseTList> {
        typedef ElementType Type;
        typedef ElementInfo<ElementType> Info;
        using BaseList = BaseTList;
        static const std::size_t elementType = Type::template idOf<Type>();
        static bool is(std::size_t typeToCheck) {
            auto curr = typeToCheck == elementType;
            if (curr) {
                return true;
            }
            
            return BaseInfo<BaseList>::is(typeToCheck);
        }
    };

    struct AbstractDataFunctor {
        virtual std::string getData() = 0;
        virtual void setData(std::string data) = 0;
    };

    typedef std::pair<std::string, std::shared_ptr<AbstractDataFunctor>> DataPair;
    typedef std::vector<DataPair> DataList;

    inline DataPair createDataPair(const char* name, AbstractDataFunctor* functor) {
        return std::make_pair<std::string, std::shared_ptr<AbstractDataFunctor>>(name, std::shared_ptr<AbstractDataFunctor>(functor));
    }
}
