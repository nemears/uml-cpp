#pragma once

#include "egm/egm-basic.h"
#include "uml/util/indexableSet.h"

namespace UML {
    template <class>
    class PackageableElement;

    template <class>
    class Namespace;

    template <class>
    class Stereotype;

    template <class ManagerPolicy>
    class Package : public ManagerPolicy {
        friend struct EGM::ElementInfo<Package>;
        public:
           using Info = EGM::TypeInfo<Package, EGM::TemplateTypeList<PackageableElement, Namespace>>; 
        protected:
           using PackageableElementSet = IndexableSet<PackageableElement, Package>;
           using OwnedStereotypes = IndexableSet<Stereotype, Package>;
           PackageableElementSet m_packagedElements = PackageableElementSet(this);
           OwnedStereotypes m_ownedStereotypes = OwnedStereotypes(this);
           void init() {
                m_packagedElements.subsets(ManagerPolicy::m_ownedMembers);
                m_packagedElements.opposite(&PackageableElementSet::ManagedType::getOwningPackageSingleton);
                m_ownedStereotypes.subsets(m_packagedElements);
           }
       public:
           MANAGED_ELEMENT_CONSTRUCTOR(Package);
           PackageableElementSet& getPackagedElements() {
                return m_packagedElements;
           }
           OwnedStereotypes& getOwnedStereotypes() { return m_ownedStereotypes; }
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
                make_set_pair("packagedElements", el.m_packagedElements),
                make_set_pair("ownedStereotypes", el.m_ownedStereotypes)
            };
        }
    };
}
