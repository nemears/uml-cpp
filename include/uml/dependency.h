#ifndef DEPENDENCY_H
#define DEPENDENCY_H

#include "packageableElement.h"
#include "directedRelationship.h"

namespace UML {
    class Dependency : public DirectedRelationship, public PackageableElement {

        friend class UmlManager;

        protected:
            Set<NamedElement, Dependency> m_client = Set<NamedElement, Dependency>(this);
            Set<NamedElement, Dependency> m_supplier = Set<NamedElement, Dependency>(this);
            void referencingReleased(ID id) override;
            void referenceReindexed(ID oldID, ID newID) override;
            void restoreReference(Element* el) override;
            void referenceErased(ID id) override;
            void init();
            void copy(const Dependency& rhs);
            Dependency();
        public:
            Dependency(const Dependency& rhs);
            virtual ~Dependency();
            Set<NamedElement, Dependency>& getClient();
            Set<NamedElement, Dependency>& getSupplier();
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::DEPENDENCY;
            };
    };
}

#endif