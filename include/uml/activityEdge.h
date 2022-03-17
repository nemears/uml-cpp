#ifndef _UML_ACTIVITY_EDGE_H_
#define _UML_ACTIVITY_EDGE_H_

#include "redefinableElement.h"
#include "valueSpecification.h"

namespace UML {

    class ActivityNode;
    class Activity;

    typedef UmlPtr<Activity> ActivityPtr;
    typedef UmlPtr<ActivityNode> ActivityNodePtr;
    typedef UmlPtr<ValueSpecification> ValueSpecificationPtr;

    class ActivityEdge : public RedefinableElement {

        friend class Activity;
        friend class ActivityNode;

        protected:

            friend class UmlManager;

            Singleton<Activity, ActivityEdge> m_activity = Singleton<Activity, ActivityEdge>(this);
            Singleton<ActivityNode, ActivityEdge> m_source = Singleton<ActivityNode, ActivityEdge>(this);
            Singleton<ActivityNode, ActivityEdge> m_target = Singleton<ActivityNode, ActivityEdge>(this);
            Singleton<ValueSpecification, ActivityEdge> m_guard = Singleton<ValueSpecification, ActivityEdge>(this);
            Singleton<ValueSpecification, ActivityEdge> m_weight = Singleton<ValueSpecification, ActivityEdge>(this);
            Set<Activity, ActivityEdge>& getActivitySingleton();
            Set<ActivityNode, ActivityEdge>& getSourceSingleton();
            Set<ActivityNode, ActivityEdge>& getTargetSingleton();
            Set<ValueSpecification, ActivityEdge>& getGuardSingleton();
            Set<ValueSpecification, ActivityEdge>& getWeightSingleton();
            void referencingReleased(ID id) override;
            void referenceReindexed(ID oldID, ID newID) override;
            void referenceErased(ID id) override;
            void restoreReference(Element* el) override;
            void init();
        public:
            ActivityEdge();
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
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::ACTIVITY_EDGE;
            };
    };
}

#endif