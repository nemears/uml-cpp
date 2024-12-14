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
            using Info = TypeInfo<NamedElement, TemplateTypeList<Element>>;
            NamedElementDefinition(std::size_t elementType, AbstractManager& manager) :
                ManagerPolicy::manager::BaseElement(elementType, manager),
                ManagerPolicy(elementType, manager)
            {}
            using NamespaceImpl = Namespace<typename ManagerPolicy::manager::template GenBaseHierarchy<Namespace>>;
            using NamespacePtr = UmlPtr<NamespaceImpl>;
            virtual ReadOnlySingleton<NamespaceDefinition, NamedElementDefinition>& getNamespaceSingleton() = 0;
            virtual NamespacePtr getNamespace() = 0;
    };

    template <>
    struct ElementInfo<NamedElement> : public DefaultInfo {};
}
