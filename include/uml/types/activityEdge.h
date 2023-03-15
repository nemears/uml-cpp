#ifndef _UML_ACTIVITY_EDGE_H_
#define _UML_ACTIVITY_EDGE_H_

#include "redefinableElement.h"
#include "valueSpecification.h"

namespace UML {

    class ActivityNode;
    class Activity;
    class InterruptibleActivityRegion;
    class ActivityGroup;
    class ActivityPartition;

    typedef UmlPtr<Activity> ActivityPtr;
    typedef UmlPtr<ActivityNode> ActivityNodePtr;
    typedef UmlPtr<ValueSpecification> ValueSpecificationPtr;
    typedef UmlPtr<InterruptibleActivityRegion> InterruptibleActivityRegionPtr;

    class ActivityEdge : public RedefinableElement {

        friend class Activity;
        friend class ActivityNode;
        friend class InterruptibleActivityRegion;

        protected:
            CustomSingleton<Activity, ActivityEdge> m_activity = CustomSingleton<Activity, ActivityEdge>(this);
            CustomSingleton<ActivityNode, ActivityEdge> m_source = CustomSingleton<ActivityNode, ActivityEdge>(this);
            CustomSingleton<ActivityNode, ActivityEdge> m_target = CustomSingleton<ActivityNode, ActivityEdge>(this);
            CustomSingleton<ValueSpecification, ActivityEdge> m_guard = CustomSingleton<ValueSpecification, ActivityEdge>(this);
            CustomSingleton<ValueSpecification, ActivityEdge> m_weight = CustomSingleton<ValueSpecification, ActivityEdge>(this);
            CustomSet<ActivityGroup, ActivityEdge> m_inGroups = CustomSet<ActivityGroup, ActivityEdge>(this);
            CustomSet<ActivityPartition, ActivityEdge> m_inPartitions = CustomSet<ActivityPartition, ActivityEdge>(this);
            CustomSingleton<InterruptibleActivityRegion, ActivityEdge> m_interrupts = CustomSingleton<InterruptibleActivityRegion, ActivityEdge>(this);
            TypedSet<Activity, ActivityEdge>& getActivitySingleton();
            TypedSet<ActivityNode, ActivityEdge>& getSourceSingleton();
            TypedSet<ActivityNode, ActivityEdge>& getTargetSingleton();
            TypedSet<ValueSpecification, ActivityEdge>& getGuardSingleton();
            TypedSet<ValueSpecification, ActivityEdge>& getWeightSingleton();
            TypedSet<InterruptibleActivityRegion, ActivityEdge>& getInterruptsSingleton();
            void referenceReindexed(ID newID) override;
            void referenceErased(ID id) override;
            ActivityEdge();
        public:
            virtual ~ActivityEdge();
            ActivityPtr getActivity() const;
            void setActivity(Activity& activity);
            void setActivity(Activity* activity);
            void setActivity(ActivityPtr activity);
            void setActivity(ID id);
            ActivityNodePtr getSource() const;
            void setSource(ActivityNode* source);
            void setSource(ActivityNode& source);
            void setSource(ActivityNodePtr source);
            void setSource(ID id);
            ActivityNodePtr getTarget() const;
            void setTarget(ActivityNode* target);
            void setTarget(ActivityNode& target);
            void setTarget(ActivityNodePtr target);
            void setTarget(ID id);
            ValueSpecificationPtr getGuard() const;
            void setGuard(ValueSpecification* guard);
            void setGuard(ValueSpecification& guard);
            void setGuard(ValueSpecificationPtr guard);
            void setGuard(ID id);
            ValueSpecificationPtr getWeight() const;
            void setWeight(ValueSpecification* weight);
            void setWeight(ValueSpecification& weight);
            void setWeight(ValueSpecificationPtr weight);
            void setWeight(ID id);
            Set<ActivityGroup, ActivityEdge>& getInGroups();
            Set<ActivityPartition, ActivityEdge>& getInPartitions();
            InterruptibleActivityRegionPtr getInterrupts() const;
            void setInterrupts(InterruptibleActivityRegion* interrupts);
            void setInterrupts(InterruptibleActivityRegion& interrupts);
            void setInterrupts(InterruptibleActivityRegionPtr interrupts);
            void setInterrupts(ID id);
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::ACTIVITY_EDGE;
            };
    };
}

#endif