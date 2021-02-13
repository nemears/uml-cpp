#ifndef ACTIVITY_H
#define ACTIVITY_H
#include "behavior.h"
#include "activityNode.h"

namespace UML {
    class Activity : public Behavior {
        public:
            Activity(){};
            ~Activity(){};
            list<ActivityNode*> nodes;
    };
}


#endif