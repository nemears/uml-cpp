#pragma once

#include <vector>
#include "uml/managers/abstractElement.h"
#include "uml/managers/templateTypeList.h"
#include "uml/managers/integerList.h"

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

    template <template <class> class ElWithSets>
    struct ElementInfo;

    template <template <class> class ElementType, class BaseTList = TemplateTypeList<>>
    struct TypeInfo : public ElementInfo<ElementType> {
        template <class ManagerPolicy>
        using Type = ElementType<ManagerPolicy>;
        using BaseList = BaseTList;
    };

    template <template<class> class Base, class ManagerPolicy>
    struct VirtualBase : public virtual Base<typename ManagerPolicy::Manager::template GenBaseHierarchy<Base>> {
        using Type = Base<typename ManagerPolicy::Manager::template GenBaseHierarchy<Base>>;
        VirtualBase(std::size_t elementType, AbstractManager& manager) : 
            ManagerPolicy::Manager::BaseElement(elementType, manager),
            Base<ManagerPolicy>(elementType, manager)
        {}
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
