#pragma once

#include "definition.h"
#include "uml/managers/dummyManager.h"
#include "uml/set/set.h"
#include "uml/types/namedElement/definition.h"

namespace UML {
    template <class ManagerPolicy>
    class Namespace : public NamespaceDefinition<ManagerPolicy> {
        protected:
            ReadOnlySet<NamedElementDefinition, NamespaceDefinition<ManagerPolicy>> m_members = ReadOnlySet<NamedElementDefinition, NamespaceDefinition<ManagerPolicy>>(this);
            // Set<NamedElementDefinition, NamespaceDefinition<ManagerPolicy>> m_members = Set<NamedElementDefinition, NamespaceDefinition<ManagerPolicy>>(this);
            // ReadOnlySet<NamedElementDefinition, NamespaceDefinition<ManagerPolicy>> m_ownedMembers = ReadOnlySet<NamedElementDefinition, NamespaceDefinition<ManagerPolicy>>(this);
            void init() {
                m_members.opposite(&decltype(m_members)::ManagedType::getNamespaceSingleton);
                // m_ownedMembers.subsets(ManagerPolicy::getOwnedElements()); 
            }
            static constexpr std::size_t namespaceElementType = ManagerPolicy::manager::template ElementType<Namespace>::result;
            Namespace() :
                ManagerPolicy::manager::BaseElement(namespaceElementType, dummyManager),
                NamespaceDefinition<ManagerPolicy>(namespaceElementType, dummyManager)
            {
                init();
            }
        public:
            // TODO delete when enforcing abstract
            Namespace(std::size_t elementType, AbstractManager& manager) :
                ManagerPolicy::manager::BaseElement(elementType, manager),
                // NamedElement<typename ManagerPolicy::Manager::template GenBaseHierarchy<NamedElement>>(elementType, manager) // virtual base
                NamespaceDefinition<ManagerPolicy>(elementType, manager)
            {
                init();
            }
            ReadOnlySet<NamedElementDefinition, NamespaceDefinition<ManagerPolicy>>& getMembers() override {
                return m_members;
            }
            // ReadOnlySet<NamedElementDefinition, NamespaceDefinition<ManagerPolicy>>& getOwnedMembers() override {
            //     return m_ownedMembers;
            // }
    };
}
