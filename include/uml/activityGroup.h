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
            Singleton<Activity, ActivityGroup> m_inActivity = Singleton<Activity, ActivityGroup>(this);
            Singleton<ActivityGroup, ActivityGroup> m_superGroup = Singleton<ActivityGroup, ActivityGroup>(this);
            Set<ActivityNode, ActivityGroup> m_containedNodes = Set<ActivityNode, ActivityGroup>(this);
            Set<ActivityEdge, ActivityGroup> m_containedEdges = Set<ActivityEdge, ActivityGroup>(this);
            Set<ActivityGroup, ActivityGroup> m_subGroups = Set<ActivityGroup, ActivityGroup>(this);
            Set<Activity, ActivityGroup>& getInActivitySingleton();
            Set<ActivityGroup, ActivityGroup>& getSuperGroupSingleton();
            void referencingReleased(ID id) override;
            void referenceReindexed(ID oldID, ID newID) override;
            void reindexName(ID id, std::string newName) override;
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