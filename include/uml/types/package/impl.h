#pragma once

#include "definition.h"
#include "uml/managers/abstractElement.h"
#include "uml/managers/dummyManager.h"
#include "uml/types/packageableElement/definition.h"

namespace UML {
    template <class ManagerPolicy>
    class Package : public PackageDefinition<ManagerPolicy> {
        protected:
            Set<PackageableElementDefinition, PackageDefinition<ManagerPolicy>> m_packagedElements = Set<PackageableElementDefinition, PackageDefinition<ManagerPolicy>>(this);
            void init() {
                m_packagedElements.opposite(&decltype(m_packagedElements)::ManagedType::getOwningPackageSingleton);
                // m_packagedElements.subsets(ManagerPolicy::getOwnedMembers()); 
            }
            // default constructor
            static constexpr std::size_t packageElementType = ManagerPolicy::Manager::template ElementType<Package>::result;
            Package() :
                ManagerPolicy::Manager::BaseElement(packageElementType, dummyManager),
                PackageDefinition<ManagerPolicy>(packageElementType, dummyManager)
            {
                init();
            }
        public:
            // most derived constructor
            Package(std::size_t elementType, AbstractManager& manager) : 
                ManagerPolicy::Manager::BaseElement(elementType, manager), // virtual base
                NamedElement<typename ManagerPolicy::Manager::template GenBaseHierarchy<NamedElement>>(elementType, manager), // virtual base
                PackageDefinition<ManagerPolicy>(elementType, manager) 
            {
                init();
            }
            Set<PackageableElementDefinition, PackageDefinition<ManagerPolicy>>& getPackagedElements() override {
                return m_packagedElements;
            }
    };
}
