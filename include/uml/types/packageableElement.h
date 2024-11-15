#pragma once

#include "uml/managers/typeInfo.h"
#include "uml/managers/templateTypeList.h"
#include "uml/managers/umlPtr.h"
#include "uml/set/singleton.h"

namespace UML {
    
    template <class>
    class NamedElement;
    template <class>
    class Package;
    template <class>
    class ParameterableElement;
    // typedef UmlPtr<Package> PackagePtr;

    template <class ManagerPolicy>
    class PackageableElement : public ManagerPolicy {

        template <class>
        friend class Package;

        friend struct ElementInfo<PackageableElement>;

        using ManagedPackage = Package<typename ManagerPolicy::Manager::template GenBaseHierarchy<Package>>;
        using PackagePtr = UmlPtr<ManagedPackage>;

        protected:
            Singleton<Package, PackageableElement> m_owningPackage = Singleton<Package, PackageableElement>(this);
            Singleton<Package, PackageableElement>& getOwningPackageSingleton() {
                return m_owningPackage;
            }
            PackageableElement(std::size_t elementType, AbstractManager& manager) : ManagerPolicy(elementType, manager) {
                m_owningPackage.subsets(ManagerPolicy::m_namespace);
                m_owningPackage.opposite(&ManagedPackage::getPackagedElements);
            
            }
        public:
            PackagePtr getOwningPackage() const {
                return m_owningPackage.get();
            }
            void setOwningPackage(ManagedPackage& package) {
                m_owningPackage.set(package);
            }
            void setOwningPackage(PackagePtr package) {
                m_owningPackage.set(package);
            }
            void setOwningPackage(ID id) {
                m_owningPackage.set(id);
            }
            typedef TypeInfo<TemplateTypeList<NamedElement/*, ParameterableElement*/>, PackageableElement> Info;
    };

    template <class ManagerPolicy>
    struct ElementInfo<PackageableElement<ManagerPolicy>> : public DefaultInfo {
        static std::string name(__attribute__((unused)) AbstractElement& el) { return "PackageableElement"; }
        static SetList sets(PackageableElement<ManagerPolicy>& el) {
            return SetList {
                std::make_pair<std::string, AbstractSet*>("owningPackage", &el.m_owningPackage)
            };
        }
    };
}
