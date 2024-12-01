#pragma once

#include "uml/managers/typeInfo.h"
#include "uml/managers/umlPtr.h"

namespace UML {
    template <class>
    class Package;

    template <class>
    class PackageableElement;

    template <class ManagerPolicy>
    class PackageableElementDefinition : virtual public ManagerPolicy {
        protected:
            using PackagePtr = UmlPtr<Package<ManagerPolicy>>;
        public:
            virtual PackagePtr getOwningPackage() = 0; 
            virtual void setOwningPackage(Package<ManagerPolicy>& package) = 0;
            virtual void setOwningPackage(PackagePtr package) = 0;
            using Info = TypeInfo<PackageableElement>;
    };
    template <>
    struct ElementInfo<PackageableElement> : public DefaultInfo {};
}
