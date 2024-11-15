#pragma once

#include <vector>
#include "uml/managers/abstractElement.h"

namespace UML {

    class AbstractSet;

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
    struct TypeInfo : public ElementInfo<ElementType> {
        typedef ElementType Type;
        typedef ElementInfo<ElementType> Info;
        using BaseList = BaseTList;
        static const constexpr std::size_t elementType = Type::template idOf<Type>();
    };

    struct AbstractDataPolicy {
        virtual std::string getData() = 0;
        virtual void setData(std::string data) = 0;
        virtual ~AbstractDataPolicy() = default;
    };

    typedef std::pair<std::string, std::shared_ptr<AbstractDataPolicy>> DataPair;
    typedef std::vector<DataPair> DataList;

    inline DataPair createDataPair(const char* name, AbstractDataPolicy* policy) {
        return std::make_pair<std::string, std::shared_ptr<AbstractDataPolicy>>(name, std::shared_ptr<AbstractDataPolicy>(policy));
    }
}
