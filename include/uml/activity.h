#ifndef _UML_ACTIVITY_H_
#define _UML_ACTIVITY_H_

#include "behavior.h"

namespace UML {

    class ActivityNode;
    class ActivityEdge;

    class Activity : public Behavior {

        friend class UmlManager;

        protected:
            Set<ActivityNode, Activity> m_nodes = Set<ActivityNode, Activity>(this);
            Set<ActivityEdge, Activity> m_edges = Set<ActivityEdge, Activity>(this);
            void init();
        public:
            Activity();
            virtual ~Activity();
            Set<ActivityNode, Activity>& getNodes();
            Set<ActivityEdge, Activity>& getEdges();
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::ACTIVITY;
            };
    };
}


#endif