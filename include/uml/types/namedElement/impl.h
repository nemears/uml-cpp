#pragma once

#include "definition.h"
#include "uml/set/singleton.h"
#include "uml/managers/dummyManager.h"

namespace UML {
    template <class ManagerPolicy>
    class NamedElement : public NamedElementDefinition<ManagerPolicy> {
        protected:
           // ReadOnlySingleton<NamespaceDefinition, NamedElement> m_namespace = ReadOnlySingleton<NamespaceDefinition, NamedElement>(this); 
           void init() {
                // m_namespace.subsets(ManagerPolicy::m_owner);
           }
           static constexpr std::size_t namedElementType = ManagerPolicy::Manager::template ElementType<NamedElement>::result;
           NamedElement() :
               ManagerPolicy::Manager::BaseElement(namedElementType, dummyManager),
               NamedElementDefinition<ManagerPolicy>(namedElementType, dummyManager)
           {
                init();
           }
        public:
            NamedElement(std::size_t elementType, AbstractManager& manager) :
                ManagerPolicy::Manager::BaseElement(elementType, manager),
                NamedElementDefinition<ManagerPolicy>(elementType, manager)
            {
                init();
            }
            // using NamespacePtr = NamespaceDefinition<ManagerPolicy>::NamespacePtr;
            // ReadOnlySingleton<NamespaceDefinition, NamedElement>& getNamespaceSingleton() override {
            //      return m_namespace;
            // }
            // NamespacePtr getNamespace() override {
            //      return m_namespace.get();
            // }
    };
}
