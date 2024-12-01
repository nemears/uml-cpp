#pragma once

#include "uml/managers/templateTypeList.h"
#include "uml/managers/typeInfo.h"
#include "uml/set/set.h"

namespace UML {
    template <class>
    class PackageableElementDefinition;

    template <class>
    class PackageableElement;

    template <class>
    class Package;

    template <class ManagerPolicy>
    struct PackageDefinition : public ManagerPolicy {
            virtual Set<PackageableElementDefinition, PackageDefinition>& getPackagedElements() = 0;
            using Info = TypeInfo<Package, TemplateTypeList<PackageableElement>>;
        protected:
            //using ManagerPolicy::ManagerPolicy;
            PackageDefinition(std::size_t elementType, AbstractManager& manager) : 
                ManagerPolicy::Manager::BaseElement(elementType, manager),
                ManagerPolicy(elementType, manager) 
            {}
    };

    template <>
    struct ElementInfo<Package> : public DefaultInfo {};
}
