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
            NamespaceDefinition(std::size_t elementType, AbstractManager& manager) :
                ManagerPolicy::Manager::BaseElement(elementType, manager),
                ManagerPolicy(elementType, manager)
            {};
            // virtual ReadOnlySet<NamedElementDefinition, NamespaceDefinition<ManagerPolicy>>& getMembers() = 0;
            // virtual ReadOnlySet<NamedElementDefinition, NamespaceDefinition<ManagerPolicy>>& getOwnedMembers() = 0;
            // template <class OtherManagerPolicy>
            // using VirtualNamedElement = VirtualBase<NamedElement, OtherManagerPolicy>;
            using Info = TypeInfo<Namespace, TemplateTypeList<NamedElement>>;
    };

    template <>
    struct ElementInfo<Namespace> : public DefaultInfo {};
}
