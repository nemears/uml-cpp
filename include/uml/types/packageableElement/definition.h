#pragma once

#include "uml/managers/templateTypeList.h"
#include "uml/managers/typeInfo.h"
#include "uml/managers/umlPtr.h"
#include "uml/types/namedElement/definition.h"

namespace UML {

    template <class>
    struct PackageDefinition;

    template <class>
    class Package;

    template <class>
    class PackageableElement;

    template <class>
    class NamedElement;

    template <class ManagerPolicy>
    class PackageableElementDefinition : public ManagerPolicy {
        protected:
            PackageableElementDefinition(std::size_t elementType, AbstractManager& manager) : 
                // NamedElement<typename ManagerPolicy::manager::template GenBaseHierarchy<NamedElement>>(elementType, manager), // virtual base
                ManagerPolicy(elementType, manager) 
            {}
        public:
            using PackageImpl = Package<typename ManagerPolicy::manager::template GenBaseHierarchy<Package>>;
            using PackagePtr = UmlPtr<PackageImpl>;
            virtual Singleton<PackageDefinition, PackageableElement<ManagerPolicy>>& getOwningPackageSingleton() = 0;
            virtual PackagePtr getOwningPackage() = 0; 
            virtual void setOwningPackage(PackageImpl& package) = 0;
            virtual void setOwningPackage(PackagePtr package) = 0;
            using Info = TypeInfo<PackageableElement, TemplateTypeList<NamedElement>>;
    };
    template <>
    struct ElementInfo<PackageableElement> : public DefaultInfo {};
}
