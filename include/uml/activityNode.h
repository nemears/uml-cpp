#ifndef _UML_ACTIVITY_NODE_H_
#define _UML_ACTIVITY_NODE_H_

#include "redefinableElement.h"

namespace UML {

    class Activity;
    class ActivityEdge;

    typedef UmlPtr<Activity> ActivityPtr;

    class ActivityNode : virtual public RedefinableElement {

        friend class UmlManager;
        friend class Activity;

        protected:
            Singleton<Activity, ActivityNode> m_activity = Singleton<Activity, ActivityNode>(this);
            Set<ActivityEdge, ActivityNode> m_incoming = Set<ActivityEdge, ActivityNode>(this);
            Set<ActivityEdge, ActivityNode> m_outgoing = Set<ActivityEdge, ActivityNode>(this);
            Set<Activity, ActivityNode>& getActivitySingleton();
            void referencingReleased(ID id) override;
            void referenceReindexed(ID oldID, ID newID) override;
            void reindexName(ID id, std::string newName) override;
            void referenceErased(ID id) override;
            void init();
        public:
            ActivityNode();
            virtual ~ActivityNode();
            Set<ActivityEdge, ActivityNode>& getIncoming();
            Set<ActivityEdge, ActivityNode>& getOutgoing();
            ActivityPtr getActivity() const;
            void setActivity(Activity* activity);
            void setActivity(Activity& activity);
            void setActivity(ActivityPtr activity);
            void setActivity(ID id);
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::ACTIVITY_NODE;
            };
    };
}

#endif