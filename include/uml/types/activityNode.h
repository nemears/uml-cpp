#ifndef _UML_ACTIVITY_NODE_H_
#define _UML_ACTIVITY_NODE_H_

#include "redefinableElement.h"
#include "uml/set/singleton.h"

namespace UML {

    class Activity;
    class ActivityEdge;
    class ActivityGroup;
    class ActivityPartition;
    class InterruptibleActivityRegion;

    typedef UmlPtr<Activity> ActivityPtr;

    class ActivityNode : virtual public RedefinableElement {

        friend class Activity;

        protected:
            Singleton<Activity, ActivityNode> m_activity = Singleton<Activity, ActivityNode>(this);
            Set<ActivityEdge, ActivityNode> m_incoming = Set<ActivityEdge, ActivityNode>(this);
            Set<ActivityEdge, ActivityNode> m_outgoing = Set<ActivityEdge, ActivityNode>(this);
            CustomReadOnlySet<ActivityGroup, ActivityNode> m_inGroups = CustomReadOnlySet<ActivityGroup, ActivityNode>(this);
            Set<ActivityPartition, ActivityNode> m_inPartitions = Set<ActivityPartition, ActivityNode>(this);
            Set<InterruptibleActivityRegion, ActivityNode> m_interruptibleRegions = Set<InterruptibleActivityRegion, ActivityNode>(this);
            Singleton<Activity, ActivityNode>& getActivitySingleton();
            void referenceErased(ID id) override;
            ActivityNode();
        public:
            virtual ~ActivityNode();
            Set<ActivityEdge, ActivityNode>& getIncoming();
            Set<ActivityEdge, ActivityNode>& getOutgoing();
            ActivityPtr getActivity() const;
            void setActivity(Activity* activity);
            void setActivity(Activity& activity);
            void setActivity(ActivityPtr activity);
            void setActivity(ID id);
            ReadOnlySet<ActivityGroup, ActivityNode>& getInGroups();
            Set<ActivityPartition, ActivityNode>& getInPartitions();
            Set<InterruptibleActivityRegion, ActivityNode>& getInterruptibleRegions();
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::ACTIVITY_NODE;
            };
    };
}

#endif
