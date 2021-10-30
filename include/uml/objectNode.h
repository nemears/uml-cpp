#ifndef OBJECTNODE_H
#define OBJECTNODE_H

#include "activityNode.h"
#include "typedElement.h"
#include "valueSpecification.h"

namespace UML {
    class ObjectNode : public ActivityNode, public TypedElement {

        friend class UmlManager;

        protected:
            void referencingReleased(ID id) override;
            void referenceReindexed(ID oldID, ID newID) override;
            void restoreReferences() override {}; // TODO
            void referenceErased(ID id) override {}; // TODO
            ValueSpecification* upperBound;
            ObjectNode();
        public:
            ObjectNode(const ObjectNode& rhs);
            ValueSpecification* getUpperBound();
            void setUpperBound(ValueSpecification* upperBound);
            bool isObjectNode() override;
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::OBJECT_NODE;
            };
    };
}

#endif