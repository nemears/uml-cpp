#ifndef _UML_DEPENDENCY_H_
#define _UML_DEPENDENCY_H_

#include "packageableElement.h"
#include "directedRelationship.h"
#include "package.h"

namespace UML {
    class Dependency : public DirectedRelationship, public PackageableElement {

        friend class UmlManager;

        protected:
            Set<NamedElement, Dependency> m_clients = Set<NamedElement, Dependency>(this);
            Set<NamedElement, Dependency> m_suppliers = Set<NamedElement, Dependency>(this);
            void referencingReleased(ID id) override;
            void referenceReindexed(ID oldID, ID newID) override;
            void reindexName(ID id, std::string newName) override;
            void restoreReference(Element* el) override;
            void referenceErased(ID id) override;
            void init();
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