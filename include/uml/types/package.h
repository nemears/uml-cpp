#pragma once

#include "uml/managers/templateTypeList.h"
#include "uml/managers/typeInfo.h"
#include "uml/set/set.h"

namespace UML {
    template <class>
    class PackageableElement;

    template <class>
    class Namespace;

    template <class ManagerPolicy>
    class Package : public ManagerPolicy {
        public:
           using Info = TypeInfo<Package, TemplateTypeList<PackageableElement, Namespace>>; 
        protected:
           using PackageableElementSet = Set<PackageableElement, Package>;
           PackageableElementSet m_packagedElements = PackageableElementSet(this);
           void init() {
                m_packagedElements.subsets(ManagerPolicy::m_ownedMembers);
                m_packagedElements.opposite(&PackageableElementSet::ManagedType::getOwningPackageSingleton);
           }
       public:
           MANAGED_ELEMENT_CONSTRUCTOR(Package);
           PackageableElementSet& getPackagedElements() {
                return m_packagedElements;
           }
    };

    template <>
    struct ElementInfo<Package> : public DefaultInfo {
        static const bool abstract = false;
        static const std::string name() {
            return "Package";
        }
    };
}
