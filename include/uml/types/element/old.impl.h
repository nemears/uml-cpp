#pragma once

#include "uml/managers/typeInfo.h"
#include "uml/set/set.h"
#include "uml/set/singleton.h"
#include <cstddef>

namespace UML {

    template <class>
    class Element;

    template <class ManagerPolicy>
    class ElementDefinition : public ManagerPolicy {
        public:
            // using ElementImpl = Element<typename ManagerPolicy::Manager::template GenBaseHierarchy<Element>>; 
            using ElementImpl = Element<ManagerPolicy>;
            using ElementPtr = UmlPtr<ElementImpl>;
            ElementDefinition(std::size_t elementType, AbstractManager& manager) :
                ManagerPolicy::manager::BaseElement(elementType, manager),
                ManagerPolicy(elementType, manager)
            {}
            virtual ReadOnlySet<ElementDefinition, ElementDefinition>& getOwnedElements() = 0;
            virtual ReadOnlySingleton<ElementDefinition, Element<ManagerPolicy>>& getOwnerSingleton() = 0;
            virtual ElementPtr getOwner() = 0;
            using Info = TypeInfo<Element>;
    };

    template <>
    struct ElementInfo<Element> : public DefaultInfo {};
}
