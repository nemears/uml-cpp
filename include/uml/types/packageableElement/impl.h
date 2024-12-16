#pragma once

#include "uml/managers/dummyManager.h"
#include "uml/set/singleton.h"
#include "definition.h"
#include "uml/types/package/definition.h"

namespace UML {
    template <class ManagerPolicy>
    class PackageableElement: public PackageableElementDefinition<ManagerPolicy> {
        protected:
            Singleton<PackageDefinition, PackageableElement> m_owningPackage = Singleton<PackageDefinition, PackageableElement>(this);
            void init() {
                m_owningPackage.opposite(&decltype(m_owningPackage)::ManagedType::getPackagedElements);
                // m_owningPackage.subsets(ManagerPolicy::getNamespaceSingleton()); 
            }
            static constexpr std::size_t packageableElementElementType = ManagerPolicy::manager::template ElementType<PackageableElement>::result;
            // default constructor
            PackageableElement() :
                ManagerPolicy::manager::BaseElement(packageableElementElementType, dummyManager),
                PackageableElementDefinition<ManagerPolicy>(packageableElementElementType, dummyManager)
            {
                init();
            }
        public:
            using PackageImpl = PackageableElementDefinition<ManagerPolicy>::PackageImpl;
            using PackagePtr = PackageableElementDefinition<ManagerPolicy>::PackagePtr;
            // most derived constructor TODO delete when this is an abstractType
            PackageableElement(std::size_t elementType, AbstractManager& manager) : 
                ManagerPolicy::manager::BaseElement(elementType, manager), // virtual base
                PackageableElementDefinition<ManagerPolicy>(elementType, manager) 
            {
                init();
            }
            Singleton<PackageDefinition, PackageableElement>& getOwningPackageSingleton() override {
                return m_owningPackage;
            }
            PackagePtr getOwningPackage() override {
                return m_owningPackage.get();
            }
            void setOwningPackage(PackageImpl& package) override {
                m_owningPackage.set(package);
            }
            void setOwningPackage(PackagePtr package) override {
                m_owningPackage.set(package);
            }
    };
}
