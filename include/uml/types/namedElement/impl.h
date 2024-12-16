#pragma once

#include "definition.h"
#include "uml/set/singleton.h"
#include "uml/managers/dummyManager.h"
#include "uml/types/namespace/definition.h"

namespace UML {
    template <class ManagerPolicy>
    class NamedElement : public NamedElementDefinition<ManagerPolicy> {
        protected:
           ReadOnlySingleton<NamespaceDefinition, NamedElementDefinition<ManagerPolicy>> m_namespace = ReadOnlySingleton<NamespaceDefinition, NamedElementDefinition<ManagerPolicy>>(this); 
           void init() {
                m_namespace.subsets(ManagerPolicy::m_owner);
           }
           static constexpr std::size_t namedElementType = ManagerPolicy::manager::template ElementType<NamedElement>::result;
           NamedElement() :
               ManagerPolicy::manager::BaseElement(namedElementType, dummyManager),
               NamedElementDefinition<ManagerPolicy>(namedElementType, dummyManager)
           {
                init();
           }
        public:
            NamedElement(std::size_t elementType, AbstractManager& manager) :
                ManagerPolicy::manager::BaseElement(elementType, manager),
                NamedElementDefinition<ManagerPolicy>(elementType, manager)
            {
                init();
            }
            using NamespacePtr = NamedElementDefinition<ManagerPolicy>::NamespacePtr;
            ReadOnlySingleton<NamespaceDefinition, NamedElementDefinition<ManagerPolicy>>& getNamespaceSingleton() override {
                 return m_namespace;
            }
            NamespacePtr getNamespace() override {
                 return m_namespace.get();
            }
    };
}
