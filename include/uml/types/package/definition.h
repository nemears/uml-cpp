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
    struct PackageDefinition : virtual public ManagerPolicy {
        // using PackageImpl = PackageDefinition<typename ManagerPolicy::Manager::template GenBaseHierarchy<PackageDefinition>>;
        virtual Set<PackageableElementDefinition, PackageDefinition>& getPackagedElements() = 0;
        using Info = TypeInfo<Package, TemplateTypeList<PackageableElement>>;
    };

    template <>
    struct ElementInfo<Package> : public DefaultInfo {};
}
