#ifndef ACTIVITYNODE_H
#define ACTIVITYNODE_H

#include "namedElement.h"
#include "activityEdge.h"

namespace UML {

    class Activity;

    class ActivityNode : virtual public NamedElement {
        protected:
            Activity* activity;
        public:
            list<ActivityEdge*> incoming;
            list<ActivityEdge*> outgoing;
            ActivityNode(){};
            ElementType getElementType() override;
            Activity* getActivity();
            void setActivity(Activity* activity);
            virtual bool isObjectNode();
    };
}

#endif