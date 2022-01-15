#ifndef ACTIVITY_H
#define ACTIVITY_H

#include "behavior.h"

namespace UML {

    class ActivityNode;
    class ActivityEdge;

    class Activity : public Behavior {

        friend class UmlManager;

        protected:
            // Set<ActivityNode, Activity> m_nodes = Set<ActivityNode, Activity>(this);
            // Set<ActivityEdge, Activity> m_edges = Set<ActivityEdge, Activity>(this);
            void referencingReleased(ID id) override;
            void referenceReindexed(ID oldID, ID newID) override;
        public:
            Activity();
            Activity(const Activity& rhs);
            virtual ~Activity();
            Set<ActivityNode, Activity>& getNodes();
            Set<ActivityNode, Activity>& getEdges();
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::ACTIVITY;
            };
    };
}


#endif