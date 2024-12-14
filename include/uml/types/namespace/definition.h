#pragma once

#include "uml/managers/abstractManager.h"
#include "uml/managers/templateTypeList.h"
#include "uml/managers/typeInfo.h"
#include "uml/set/set.h"
#include <cstddef>

namespace UML {

    template <class>
    class NamedElementDefinition;

    template <class>
    class NamedElement;

    template <class>
    class Namespace;

    template <class ManagerPolicy>
    class NamespaceDefinition : public ManagerPolicy {
        public:
            // virtual ReadOnlySet<NamedElementDefinition, NamespaceDefinition>& getMembers() = 0;
            using Info = TypeInfo<Namespace, TemplateTypeList<NamedElement>>;
            NamespaceDefinition(std::size_t elementType, AbstractManager& manager) :
                ManagerPolicy::manager::BaseElement(elementType, manager),
                ManagerPolicy(elementType, manager)
            {};
            // virtual ReadOnlySet<NamedElementDefinition, NamespaceDefinition>& getOwnedMembers() = 0;
    };

    template <>
    struct ElementInfo<Namespace> : public DefaultInfo {};
}
