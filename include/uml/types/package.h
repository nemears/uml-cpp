#pragma once

#include "egm/egm-basic.h"

namespace UML {
    template <class>
    class PackageableElement;

    template <class>
    class Namespace;

    template <class ManagerPolicy>
    class Package : public ManagerPolicy {
        friend struct EGM::ElementInfo<Package>;
        public:
           using Info = EGM::TypeInfo<Package, EGM::TemplateTypeList<PackageableElement, Namespace>>; 
        protected:
           using PackageableElementSet = EGM::Set<PackageableElement, Package>;
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
}

namespace EGM {
    template <>
    struct ElementInfo<UML::Package> {
        static const std::string name() {
            return "Package";
        }
        template <class Policy>
        static SetList sets(UML::Package<Policy>& el) {
            return SetList {
                make_set_pair("packagedElements", el.m_packagedElements)
            };
        }
    };
}
