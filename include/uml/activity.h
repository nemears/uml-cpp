#ifndef ACTIVITY_H
#define ACTIVITY_H
#include "behavior.h"
#include "activityNode.h"
#include "activityEdge.h"

namespace UML {
    class Activity : public Behavior {
        protected:
            Sequence<ActivityNode>* m_nodes;
            Sequence<ActivityEdge>* m_edges;
        public:
            Activity();
            ~Activity();
            Sequence<ActivityNode>& getNodes();
            Sequence<ActivityEdge>& getEdges();
            ElementType getElementType() override;
    };
}


#endif