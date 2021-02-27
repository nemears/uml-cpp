#ifndef ACTIVITYEDGE_H
#define ACTIVITYEDGE_H

#include "namedElement.h"
#include "valueSpecification.h"

namespace UML {

    // forward declaration
    class ActivityNode;

    class ActivityEdge : public NamedElement {
        public:
            ActivityNode* getSource();
            void setSource(ActivityNode* source);
            ActivityNode* getTarget();
            void setTarget(ActivityNode* target);
            ValueSpecification* getGuard();
            void setGuard(ValueSpecification* guard);
            ActivityEdge() {
                source = 0;
                target = 0;
                guard = 0;
            }
            ElementType getElementType() override;

        protected:
            ActivityNode* source;
            ActivityNode* target;
            ValueSpecification* guard;
    };
}

#endif