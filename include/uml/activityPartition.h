#ifndef _UML_ACTIVITY_PARTITION_H_
#define _UML_ACTIVITY_PARTITION_H_

#include "activityGroup.h"

namespace UML {

    class ActivityPartition;
    typedef UmlPtr<ActivityPartition> ActivityPartitionPtr;

    class ActivityPartition : public ActivityGroup {

        friend class Activity;
        template <typename AccessPolicy, typename PersistencePolicy> friend class Manager;

        protected:
            CustomSingleton<Element, ActivityPartition> m_represents = CustomSingleton<Element, ActivityPartition>(this);
            CustomSingleton<ActivityPartition, ActivityPartition> m_superPartition = CustomSingleton<ActivityPartition, ActivityPartition>(this);
            CustomSet<ActivityPartition, ActivityPartition> m_subPartitions = CustomSet<ActivityPartition, ActivityPartition>(this);
            CustomSet<ActivityNode, ActivityPartition> m_nodes = CustomSet<ActivityNode, ActivityPartition>(this);
            CustomSet<ActivityEdge, ActivityPartition> m_edges = CustomSet<ActivityEdge, ActivityPartition>(this);
            CustomSingleton<Activity, ActivityPartition> m_partitionInActivity = CustomSingleton<Activity, ActivityPartition>(this);
            TypedSet<Element, ActivityPartition>& getRepresentsSingleton();
            TypedSet<Activity, ActivityPartition>& getInActivitySingleton();
            TypedSet<ActivityPartition, ActivityPartition>& getSuperPartitionSingleton();
            void referenceReindexed(ID oldID, ID newID) override;
            void referenceErased(ID id) override;
            void init();
            ActivityPartition();
        public:
            virtual ~ActivityPartition();
            ElementPtr getRepresents() const;
            void setRepresents(Element* represents);
            void setRepresents(Element& represents);
            void setRepresents(ElementPtr represents);
            void setRepresents(ID id);
            ActivityPartitionPtr getSuperPartition() const;
            void setSuperPartition(ActivityPartition* superPartition);
            void setSuperPartition(ActivityPartition& superPartition);
            void setSuperPartition(ActivityPartitionPtr superPartition);
            void setSuperPartition(ID id);
            Set<ActivityPartition, ActivityPartition>& getSubPartitions();
            Set<ActivityNode, ActivityPartition>& getNodes();
            Set<ActivityEdge, ActivityPartition>& getEdges();
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::ACTIVITY_PARTITION;
            }
    };
}

#endif