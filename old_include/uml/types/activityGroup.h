#ifndef _UML_ACTIVITY_GROUP_H_
#define _UML_ACTIVITY_GROUP_H_

#include "activityNode.h"
#include "activityEdge.h"
#include "uml/set/singleton.h"

namespace UML {

    class ActivityGroup;
    typedef UmlPtr<Activity> ActivityPtr;
    typedef UmlPtr<ActivityGroup> ActivityGroupPtr;

    class ActivityGroup : public NamedElement {

        friend class Activity;

        protected:
            Singleton<Activity, ActivityGroup> m_inActivity = Singleton<Activity, ActivityGroup>(this);
            Singleton<ActivityGroup, ActivityGroup> m_superGroup = Singleton<ActivityGroup, ActivityGroup>(this);
            ReadOnlySet<ActivityNode, ActivityGroup> m_containedNodes = ReadOnlySet<ActivityNode, ActivityGroup>(this);
            ReadOnlySet<ActivityEdge, ActivityGroup> m_containedEdges = ReadOnlySet<ActivityEdge, ActivityGroup>(this);
            ReadOnlySet<ActivityGroup, ActivityGroup> m_subGroups = ReadOnlySet<ActivityGroup, ActivityGroup>(this);
            Singleton<Activity, ActivityGroup>& getInActivitySingleton();
            Singleton<ActivityGroup, ActivityGroup>& getSuperGroupSingleton();
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
