#ifndef _UML_DEPENDENCY_H_
#define _UML_DEPENDENCY_H_

#include "packageableElement.h"
#include "directedRelationship.h"
#include "package.h"

namespace UML {
    class Dependency : public DirectedRelationship, public PackageableElement {

        friend class UmlManager;

        protected:
            Set<NamedElement, Dependency> m_client = Set<NamedElement, Dependency>(this);
            Set<NamedElement, Dependency> m_supplier = Set<NamedElement, Dependency>(this);
            void referencingReleased(ID id) override;
            void referenceReindexed(ID oldID, ID newID) override;
            void reindexName(std::string oldName, std::string newName) override;
            void restoreReference(Element* el) override;
            void referenceErased(ID id) override;
            void init();
            Dependency();
        public:
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