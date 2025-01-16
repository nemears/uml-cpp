#pragma once

#include <memory>
#include <type_traits>
#include <vector>
#include "uml/managers/abstractElement.h"
#include "uml/managers/templateTypeList.h"
#include "uml/managers/dummyManager.h"

namespace UML {

    class AbstractSet;

    typedef std::vector<std::pair<std::string, AbstractSet*>> SetList;
    
    template <class>
    struct TemplateTrue : public std::true_type {};
        
    template <template <class> class ElWithSets>
    struct ElementInfo;

    template <template <class> class Type>
    static auto testSets(int) -> TemplateTrue<decltype(ElementInfo<Type>::sets(std::declval<Type<DummyManager::BaseElement>&>()))>;

    template <template <class> class>
    static auto testSets(...) -> std::false_type;

    template <template <class> class Type>
    struct HasSets : decltype(testSets<Type>(0)) {};

    template <template <class> class Type>
    static auto testData(int) -> TemplateTrue<decltype(ElementInfo<Type>::template data(std::declval<Type<DummyManager::BaseElement>&>()))>;

    template <template <class> class>
    static auto testData(...) -> std::false_type;

    template <template <class> class Type>
    struct HasData : decltype(testData<Type>(0)) {};

    template <template <class> class ElementType, class BaseTList = TemplateTypeList<>>
    struct TypeInfo : public ElementInfo<ElementType> {
        template <class ManagerPolicy>
        using Type = ElementType<ManagerPolicy>;
        using BaseList = BaseTList;
    };

    struct AbstractDataPolicy {
        virtual std::string getData() = 0;
        virtual void setData(std::string data) = 0;
        virtual ~AbstractDataPolicy() = default;
    };

    typedef std::pair<std::string, std::shared_ptr<AbstractDataPolicy>> DataPair;
    typedef std::vector<DataPair> DataList;

    template <class DataPolicy, class ... ConstructorArgs>
    DataPair createDataPair(const char* name, ConstructorArgs&& ... args) {
        return std::make_pair<std::string, std::shared_ptr<DataPolicy>>(name, std::make_shared<DataPolicy>(args...));
    }
}

// macro for setting up monotonous constructor needed for defining compatible policy classes
#define MANAGED_ELEMENT_CONSTRUCTOR(ElementTypeName) \
    ElementTypeName () : \
        ManagerPolicy::manager::BaseElement(0, dummyManager), \
        ManagerPolicy(0, dummyManager) \
    { \
        init(); \
    }; \
    ElementTypeName (std::size_t elementType, AbstractManager& manager) : \
        ManagerPolicy::manager::BaseElement(elementType, manager), \
        ManagerPolicy(elementType, manager) \
    { \
        init(); \
    } 
