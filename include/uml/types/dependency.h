#ifndef _UML_DEPENDENCY_H_
#define _UML_DEPENDENCY_H_

#include "packageableElement.h"
#include "directedRelationship.h"
#include "package.h"

namespace UML {
    class Dependency : public DirectedRelationship, public PackageableElement {

        template <typename AccessPolicy, typename SerializationPolicy, typename PersistencePolicy> friend class Manager;

        protected:
            CustomSet<NamedElement, Dependency> m_clients = CustomSet<NamedElement, Dependency>(this);
            CustomSet<NamedElement, Dependency> m_suppliers = CustomSet<NamedElement, Dependency>(this);
            void referenceReindexed(ID newID) override;
            void referenceErased(ID id) override;
            Dependency();
        public:
            virtual ~Dependency();
            Set<NamedElement, Dependency>& getClients();
            Set<NamedElement, Dependency>& getSuppliers();
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::DEPENDENCY;
            };
    };
}

#endif