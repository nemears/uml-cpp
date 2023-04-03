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
            CustomReadOnlySet<ActivityNode, ActivityGroup> m_containedNodes = CustomReadOnlySet<ActivityNode, ActivityGroup>(this);
            CustomReadOnlySet<ActivityEdge, ActivityGroup> m_containedEdges = CustomReadOnlySet<ActivityEdge, ActivityGroup>(this);
            CustomReadOnlySet<ActivityGroup, ActivityGroup> m_subGroups = CustomReadOnlySet<ActivityGroup, ActivityGroup>(this);
            TypedSet<Activity, ActivityGroup>& getInActivitySingleton();
            TypedSet<ActivityGroup, ActivityGroup>& getSuperGroupSingleton();
            void referenceReindexed(ID newID) override;
            void referenceErased(ID id) override;
            ActivityGroup();
        public:
            virtual ~ActivityGroup();
            ActivityPtr getInActivity() const;
            void setInActivity(Activity* inActivity);
            void setInActivity(Activity& inActivity);
            void setInActivity(ActivityPtr inActivity);
            void setInActivity(ID id);
            ActivityGroupPtr getSuperGroup() const;
            ReadOnlySet<ActivityNode, ActivityGroup>& getContainedNodes();
            ReadOnlySet<ActivityEdge, ActivityGroup>& getContainedEdges();
            ReadOnlySet<ActivityGroup, ActivityGroup>& getSubGroups();
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::ACTIVITY_GROUP;
            };
    };
}

#endif