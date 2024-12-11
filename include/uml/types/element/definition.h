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
            // using ElementImpl = ElementDefinition<ManagerPolicy>::Info::template Type<ManagerPolicy>; 
            // using ElementPtr = UmlPtr<ElementImpl>;
            ElementDefinition(std::size_t elementType, AbstractManager& manager) :
                ManagerPolicy::Manager::BaseElement(elementType, manager),
                ManagerPolicy(elementType, manager)
            {}
            // virtual ReadOnlySet<ElementDefinition, ElementImpl>& getOwnedElements() = 0;
            // virtual ReadOnlySingleton<ElementDefinition, ElementImpl>& getOwnerSingleton() = 0;
            // virtual ElementPtr getOwner() = 0;
            using Info = TypeInfo<Element>;
    };

    template <>
    struct ElementInfo<Element> : public DefaultInfo {};
}
