#pragma once

#include "uml/managers/typeInfo.h"
#include "uml/managers/umlPtr.h"

namespace UML {
    template <class>
    class Package;

    template <class>
    class PackageableElement;

    template <class ManagerPolicy>
    class PackageableElementDefinition : public ManagerPolicy {
        protected:
            using PackageImpl = Package<typename ManagerPolicy::Manager::template GenBaseHierarchy<Package>>;
            using PackagePtr = UmlPtr<PackageImpl>;
            using ManagerPolicy::ManagerPolicy;
            PackageableElementDefinition(std::size_t elementType, AbstractManager& manager) : 
                ManagerPolicy::Manager::BaseElement(elementType, manager),
                ManagerPolicy(elementType, manager) 
            {} 
        public:
            virtual PackagePtr getOwningPackage() = 0; 
            virtual void setOwningPackage(PackageImpl& package) = 0;
            virtual void setOwningPackage(PackagePtr package) = 0;
            using Info = TypeInfo<PackageableElement>;
    };
    template <>
    struct ElementInfo<PackageableElement> : public DefaultInfo {};
}
