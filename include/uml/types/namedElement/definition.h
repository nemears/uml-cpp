#pragma once

#include "uml/managers/templateTypeList.h"
#include "uml/managers/typeInfo.h"
#include "uml/set/singleton.h"
#include <cstddef>

namespace UML {

    template <class>
    class Element;

    template <class>
    class NamedElement;

    template <class>
    class NamespaceDefinition;

    template <class>
    class Namespace;

    template <class ManagerPolicy>
    class NamedElementDefinition : public ManagerPolicy {
        public:
            NamedElementDefinition(std::size_t elementType, AbstractManager& manager) :
                ManagerPolicy::Manager::BaseElement(elementType, manager),
                ManagerPolicy(elementType, manager)
            {}
            // using NamedElementImpl = NamedElement<typename ManagerPolicy::Manager::template GenBaseHierarchy<NamedElement>>;
            // using NamespaceImpl = Namespace<typename ManagerPolicy::Manager::template GenBaseHierarchy<Namespace>>;
            // using NamespacePtr = UmlPtr<NamespaceImpl>;
            // virtual ReadOnlySingleton<NamespaceDefinition, NamedElementImpl>& getNamespaceSingleton() = 0;
            // virtual NamespacePtr getNamespace() = 0;
            using Info = TypeInfo<NamedElement, TemplateTypeList<Element>>;
    };

    template <>
    struct ElementInfo<NamedElement> : public DefaultInfo {};
}
