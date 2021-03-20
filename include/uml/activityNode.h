#ifndef ACTIVITYNODE_H
#define ACTIVITYNODE_H

#include "namedElement.h"
#include "activityEdge.h"

namespace UML {

    class Activity;

    class ActivityNode : virtual public NamedElement {
        protected:
            Activity* m_activity;
            Sequence<ActivityEdge>* m_incoming;
            Sequence<ActivityEdge>* m_outgoing;
            void reindexID(boost::uuids::uuid oldID, boost::uuids::uuid newID) override;
            void reindexName(string oldName, string newName) override;
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