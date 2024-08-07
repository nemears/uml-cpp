#pragma once
#include "packageableElement.h"
#include "directedRelationship.h"

namespace UML {
    class Dependency : public DirectedRelationship, public PackageableElement {

        template <typename SerializationPolicy, typename PersistencePolicy> friend class Manager;

        protected:
            Set<NamedElement, Dependency> m_clients = Set<NamedElement, Dependency>(this);
            Set<NamedElement, Dependency> m_suppliers = Set<NamedElement, Dependency>(this);
            void referenceErased(ID id) override;
            Dependency();
        public:
            virtual ~Dependency();
            Set<NamedElement, Dependency>& getClients();
            Set<NamedElement, Dependency>& getSuppliers();
            bool is(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::DEPENDENCY;
            };
    };
}
