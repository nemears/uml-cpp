#ifndef _UML_ACTIVITY_H_
#define _UML_ACTIVITY_H_

#include "behavior.h"
#include "activityNode.h"
#include "activityEdge.h"
#include "activityGroup.h"
#include "activityPartition.h"
#include "interruptibleActivityRegion.h"

namespace UML {

    class ActivityNode;
    class ActivityEdge;
    class ActivityGroup;

    class Activity : public Behavior {

        template <typename AccessPolicy, typename SerializationPolicy, typename PersistencePolicy> friend class Manager;

        protected:
            CustomSet<ActivityNode, Activity> m_nodes = CustomSet<ActivityNode, Activity>(this);
            CustomSet<ActivityEdge, Activity> m_edges = CustomSet<ActivityEdge, Activity>(this);
            CustomSet<ActivityGroup, Activity> m_groups = CustomSet<ActivityGroup, Activity>(this);
            CustomSet<ActivityPartition, Activity> m_partitions = CustomSet<ActivityPartition, Activity>(this);
            Activity();
        public:
            virtual ~Activity();
            Set<ActivityNode, Activity>& getNodes();
            Set<ActivityEdge, Activity>& getEdges();
            Set<ActivityGroup, Activity>& getGroups();
            Set<ActivityPartition, Activity>& getPartitions();
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::ACTIVITY;
            };
    };
}


#endif