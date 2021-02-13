#ifndef ACTIVITYEDGE_H
#define ACTIVITYEDGE_H

#include "namedElement.h"

namespace UML {

    // forward declaration
    class ActivityNode;

    class ActivityEdge : public NamedElement {
        public:
            ActivityNode* getSource();
            void setSource(ActivityNode* source);
            ActivityNode* getTarget();
            void setTarget(ActivityNode* target);

        protected:
            ActivityNode* source;
            ActivityNode* target;
    };
}

#endif