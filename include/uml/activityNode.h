#ifndef ACTIVITYNODE_H
#define ACTIVITYNODE_H

#include "namedElement.h"
#include "activityEdge.h"

namespace UML {

    class Activity;

    class ActivityNode : virtual public NamedElement {
        protected:
            Activity* activity;
            Sequence<ActivityEdge>* m_incoming;
            Sequence<ActivityEdge>* m_outgoing;
        public:
            ActivityNode();
            ~ActivityNode();
            Sequence<ActivityEdge>& getIncoming();
            Sequence<ActivityEdge>& getOutgoing();
            ElementType getElementType() override;
            Activity* getActivity();
            void setActivity(Activity* activity);
            virtual bool isObjectNode();
    };
}

#endif