#ifndef _UML_ACTIVITY_GROUP_H_
#define _UML_ACTIVITY_GROUP_H_

#include "activityNode.h"
#include "activityEdge.h"

namespace UML {

    class ActivityGroup;
    typedef UmlPtr<Activity> ActivityPtr;
    typedef UmlPtr<ActivityGroup> ActivityGroupPtr;

    class ActivityGroup : public NamedElement {

        friend class Activity;

        protected:
            CustomSingleton<Activity, ActivityGroup> m_inActivity = CustomSingleton<Activity, ActivityGroup>(this);
            CustomSingleton<ActivityGroup, ActivityGroup> m_superGroup = CustomSingleton<ActivityGroup, ActivityGroup>(this);
            CustomSet<ActivityNode, ActivityGroup> m_containedNodes = CustomSet<ActivityNode, ActivityGroup>(this);
            CustomSet<ActivityEdge, ActivityGroup> m_containedEdges = CustomSet<ActivityEdge, ActivityGroup>(this);
            CustomSet<ActivityGroup, ActivityGroup> m_subGroups = CustomSet<ActivityGroup, ActivityGroup>(this);
            TypedSet<Activity, ActivityGroup>& getInActivitySingleton();
            TypedSet<ActivityGroup, ActivityGroup>& getSuperGroupSingleton();
            void referenceReindexed(ID newID) override;
            void referenceErased(ID id) override;
            void init();
            ActivityGroup();
        public:
            virtual ~ActivityGroup();
            ActivityPtr getInActivity() const;
            void setInActivity(Activity* inActivity);
            void setInActivity(Activity& inActivity);
            void setInActivity(ActivityPtr inActivity);
            void setInActivity(ID id);
            ActivityGroupPtr getSuperGroup() const;
            Set<ActivityNode, ActivityGroup>& getContainedNodes();
            Set<ActivityEdge, ActivityGroup>& getContainedEdges();
            Set<ActivityGroup, ActivityGroup>& getSubGroups();
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::ACTIVITY_GROUP;
            };
    };
}

#endif