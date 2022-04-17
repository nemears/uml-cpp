#ifndef _UML_ACTIVITY_PARTITION_H_
#define _UML_ACTIVITY_PARTITION_H_

#include "activityGroup.h"

namespace UML {

    class ActivityPartition;
    typedef UmlPtr<ActivityPartition> ActivityPartitionPtr;

    class ActivityPartition : public ActivityGroup {

        friend class Activity;
        friend class UmlManager;

        protected:
            Singleton<Element, ActivityPartition> m_represents = Singleton<Element, ActivityPartition>(this);
            Singleton<ActivityPartition, ActivityPartition> m_superPartition = Singleton<ActivityPartition, ActivityPartition>(this);
            Set<ActivityPartition, ActivityPartition> m_subPartitions = Set<ActivityPartition, ActivityPartition>(this);
            Set<ActivityNode, ActivityPartition> m_nodes = Set<ActivityNode, ActivityPartition>(this);
            Set<ActivityEdge, ActivityPartition> m_edges = Set<ActivityEdge, ActivityPartition>(this);
            Singleton<Activity, ActivityPartition> m_partitionInActivity = Singleton<Activity, ActivityPartition>(this);
            Set<Element, ActivityPartition>& getRepresentsSingleton();
            Set<Activity, ActivityPartition>& getInActivitySingleton();
            Set<ActivityPartition, ActivityPartition>& getSuperPartitionSingleton();
            void referencingReleased(ID id) override;
            void referenceReindexed(ID oldID, ID newID) override;
            void reindexName(ID id, std::string newName) override;
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