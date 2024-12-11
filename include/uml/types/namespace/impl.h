#pragma once

#include "definition.h"
#include "uml/managers/dummyManager.h"
#include "uml/set/set.h"

namespace UML {
    template <class ManagerPolicy>
    class Namespace : public NamespaceDefinition<ManagerPolicy> {
        protected:
            // ReadOnlySet<NamespaceDefinition, Namespace> m_members = ReadOnlySet<NamespaceDefinition, Namespace>(this);
            // ReadOnlySet<NamespaceDefinition, Namespace> m_ownedMembers = ReadOnlySet<NamespaceDefinition, Namespace>(this);
            void init() {
                // m_members.opposite(&decltype(m_members)::ManagedElement::getNamespaceSingleton);
                // m_ownedMembers.subsets(ManagerPolicy::getOwnedElements()); 
            }
            static constexpr std::size_t namespaceElementType = ManagerPolicy::Manager::template ElementType<Namespace>::result;
            Namespace() :
                ManagerPolicy::Manager::BaseElement(namespaceElementType, dummyManager),
                NamespaceDefinition<ManagerPolicy>(namespaceElementType, dummyManager)
            {
                init();
            }
        public:
            // TODO delete when enforcing abstract
            Namespace(std::size_t elementType, AbstractManager& manager) :
                ManagerPolicy::Manager::BaseElement(elementType, manager),
                // NamedElement<typename ManagerPolicy::Manager::template GenBaseHierarchy<NamedElement>>(elementType, manager) // virtual base
                NamespaceDefinition<ManagerPolicy>(elementType, manager)
            {
                init();
            }
            // ReadOnlySet<NamespaceDefinition, Namespace>& getMembers() override {
            //     return m_members;
            // }
            // ReadOnlySet<NamespaceDefinition, Namespace>& getOwnedMembers() override {
            //     return m_ownedMembers;
            // }
    };
}
