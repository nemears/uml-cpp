#ifndef DEPENDENCY_H
#define DEPENDENCY_H

#include "packageableElement.h"
#include "directedRelationship.h"

namespace UML {
    class Dependency : public DirectedRelationship, public PackageableElement {
        protected:
            void setManager(UmlManager* manager) override;
            void reindexID(ID oldID, ID newID) override;
        public:
            Dependency();
            Dependency(const Dependency& dependency);
            virtual ~Dependency();
            ElementType getElementType() const override;
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::DEPENDENCY;
            };
    };
}

#endif