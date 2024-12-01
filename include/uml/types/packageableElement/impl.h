#pragma once

#include "uml/set/singleton.h"
#include "definition.h"
#include "uml/types/package/definition.h"

namespace UML {
    template <class ManagerPolicy>
    class PackageableElement: public PackageableElementDefinition<ManagerPolicy> {
        protected:
            Singleton<Package, PackageableElement> m_owningPackage = Singleton<Package, PackageableElement>(this);
        public:
            using PackagePtr = PackageableElementDefinition<ManagerPolicy>::PackagePtr;
            PackagePtr getOwningPackage() override {
                return m_owningPackage.get();
            }
            void setOwningPackage(Package<ManagerPolicy>& package) override {
                m_owningPackage.set(package);
            }
            void setOwningPackage(PackagePtr package) override {
                m_owningPackage.set(package);
            }
    };
}
