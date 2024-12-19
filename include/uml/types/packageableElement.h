#pragma once

#include "uml/managers/templateTypeList.h"
#include "uml/managers/typeInfo.h"
#include "uml/set/singleton.h"

namespace UML {
    template <class>
    class NamedElement;

    template <class>
    class Package;

    template <class ManagerPolicy>
    class PackageableElement : public ManagerPolicy {
        public:
            using Info = TypeInfo<PackageableElement, TemplateTypeList<NamedElement>>;
        protected:
            using PackageSingleton = Singleton<Package, PackageableElement>;
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
            using PackageImpl = PackageSingleton::ManagedType; 
            using PackagePtr = UmlPtr<PackageImpl>;
            PackagePtr getOwningPackage() {
                return m_owningPackage.get();
            }
            void setOwningPackage(PackageImpl& pckg) {
                m_owningPackage.set(pckg);
            }
            void setOwningPackage(PackagePtr pckg) {
                m_owningPackage.set(pckg);
            }
            void setOwningPackage(ID id) {
                m_owningPackage.set(id);
            }
    };

    template <>
    struct ElementInfo<PackageableElement> : public DefaultInfo {};
}
