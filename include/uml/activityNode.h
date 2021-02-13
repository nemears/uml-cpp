#ifndef ACTIVITYNODE_H
#define ACTIVITYNODE_H

#include "namedElement.h"
#include "activityEdge.h"

namespace UML {
    class ActivityNode : virtual public NamedElement {
        public:
            list<ActivityEdge*> incoming;
            list<ActivityEdge*> outgoing;
            ActivityNode(){};
    };
}

#endif