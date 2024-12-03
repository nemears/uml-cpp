#pragma once

#include "uml/managers/typeInfo.h"
#include "uml/managers/umlPtr.h"
#include "uml/types/package/definition.h"

namespace UML {
    template <class>
    class Package;

    template <class>
    class PackageableElement;

    template <class ManagerPolicy>
    class PackageableElementDefinition : public ManagerPolicy {
        protected:
            PackageableElementDefinition(std::size_t elementType, AbstractManager& manager) : 
                ManagerPolicy::Manager::BaseElement(elementType, manager),
                ManagerPolicy(elementType, manager) 
            {} 
        public:
            using PackageImpl = PackageDefinition<typename ManagerPolicy::Manager::template GenBaseHierarchy<PackageDefinition>>;
            using PackagePtr = UmlPtr<PackageImpl>;
            virtual PackagePtr getOwningPackage() = 0; 
            virtual void setOwningPackage(PackageImpl& package) = 0;
            virtual void setOwningPackage(PackagePtr package) = 0;
            using Info = TypeInfo<PackageableElement>;
    };
    template <>
    struct ElementInfo<PackageableElement> : public DefaultInfo {};
}
