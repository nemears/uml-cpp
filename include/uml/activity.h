#ifndef ACTIVITY_H
#define ACTIVITY_H
#include "behavior.h"
#include "activityNode.h"
#include "activityEdge.h"

namespace UML {
    class Activity : public Behavior {
        public:
            Activity(){};
            ~Activity(){};
            list<ActivityNode*> nodes;
            list<ActivityEdge*> edges;
    };
}


#endif