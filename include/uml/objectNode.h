#ifndef OBJECTNODE_H
#define OBJECTNODE_H

#include "activityNode.h"
#include "typedElement.h"
#include "valueSpecification.h"

namespace UML {
    class ObjectNode : public ActivityNode, public TypedElement {
        protected:
            ValueSpecification* upperBound;
        public:
            ElementType getElementType() override;
            ValueSpecification* getUpperBound();
            void setUpperBound(ValueSpecification* upperBound);
    };
}

#endif