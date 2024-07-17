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

        template <typename SerializationPolicy, typename PersistencePolicy> friend class Manager;

        protected:
            Set<ActivityNode, Activity> m_nodes = Set<ActivityNode, Activity>(this);
            Set<ActivityEdge, Activity> m_edges = Set<ActivityEdge, Activity>(this);
            Set<ActivityGroup, Activity> m_groups = Set<ActivityGroup, Activity>(this);
            Set<ActivityPartition, Activity> m_partitions = Set<ActivityPartition, Activity>(this);
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
