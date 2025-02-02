#pragma once

#include "egm/egm-basic.h"

namespace UML {
    template <class>
    class NamedElement;

    template <class>
    class Package;

    template <class ManagerPolicy>
    class PackageableElement : public ManagerPolicy {
        friend struct EGM::ElementInfo<PackageableElement>;
        public:
            using Info = EGM::TypeInfo<PackageableElement, EGM::TemplateTypeList<NamedElement>>;
        protected:
            using PackageSingleton = EGM::Singleton<Package, PackageableElement>;
            PackageSingleton m_owningPackage = PackageSingleton(this);
            void init() {
                m_owningPackage.subsets(ManagerPolicy::m_namespace);
                m_owningPackage.opposite(&PackageSingleton::ManagedType::getPackagedElements);
            }
       public:
            MANAGED_ELEMENT_CONSTRUCTOR(PackageableElement);
            PackageSingleton& getOwningPackageSingleton() {
                return m_owningPackage;
            }
            using PackageImpl = typename PackageSingleton::ManagedType; 
            using PackagePtr = EGM::ManagedPtr<PackageImpl>;
            PackagePtr getOwningPackage() {
                return m_owningPackage.get();
            }
            void setOwningPackage(PackageImpl& pckg) {
                m_owningPackage.set(pckg);
            }
            void setOwningPackage(PackagePtr pckg) {
                m_owningPackage.set(pckg);
            }
            void setOwningPackage(EGM::ID id) {
                m_owningPackage.set(id);
            }
    };
}

namespace EGM {
    template <>
    struct ElementInfo<UML::PackageableElement> {
        static std::string name() { return "PackageableElement"; }
        static const bool abstract = true;
        template <class Policy>
        static SetList sets(UML::PackageableElement<Policy>& el) {
            return SetList {
                make_set_pair("owningPackage", el.m_owningPackage)
            };
        }
    };
}
