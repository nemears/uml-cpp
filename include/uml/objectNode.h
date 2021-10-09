#ifndef OBJECTNODE_H
#define OBJECTNODE_H

#include "activityNode.h"
#include "typedElement.h"
#include "valueSpecification.h"

namespace UML {
    class ObjectNode : public ActivityNode, public TypedElement {
        protected:
            void referencingReleased(ID id) override;
            void referenceReindexed(ID oldID, ID newID) override;
            void restoreReferences() override {}; // TODO
            ValueSpecification* upperBound;
        public:
            ElementType getElementType() const override;
            ValueSpecification* getUpperBound();
            void setUpperBound(ValueSpecification* upperBound);
            ObjectNode() {
                upperBound = NULL;
            };
            bool isObjectNode() override;
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::OBJECT_NODE;
            };
    };
}

#endif