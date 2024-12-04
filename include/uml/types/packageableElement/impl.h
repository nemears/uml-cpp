#pragma once

#include "uml/set/singleton.h"
#include "definition.h"
#include "uml/types/package/definition.h"

namespace UML {
    template <class ManagerPolicy>
    class PackageableElement: public PackageableElementDefinition<ManagerPolicy> {
        protected:
            Singleton<PackageDefinition, PackageableElement> m_owningPackage = Singleton<PackageDefinition, PackageableElement>(this);
        public:
            // using PackageableElementDefinition<ManagerPolicy>::PackageableElementDefinition;
            PackageableElement(std::size_t elementType, AbstractManager& manager) : 
                ManagerPolicy::Manager::BaseElement(elementType, manager),
                PackageableElementDefinition<ManagerPolicy>(elementType, manager) 
            {}
            using PackageImpl = PackageableElementDefinition<ManagerPolicy>::PackageImpl;
            using PackagePtr = PackageableElementDefinition<ManagerPolicy>::PackagePtr;
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
