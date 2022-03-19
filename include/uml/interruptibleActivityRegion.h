#ifndef _UML_INTERRUPTIBLE_ACTIVITY_REGION_H_
#define _UML_INTERRUPTIBLE_ACTIVITY_REGION_H_

#include "activityGroup.h"

namespace UML {
    class InterruptibleActivityRegion : public ActivityGroup {
        protected:
            Set<ActivityNode, InterruptibleActivityRegion> m_nodes = Set<ActivityNode, InterruptibleActivityRegion>(this);
            Set<ActivityEdge, InterruptibleActivityRegion> m_interruptingEdges = Set<ActivityEdge, InterruptibleActivityRegion>(this);
            void init();
            InterruptibleActivityRegion();
        public:
            virtual ~InterruptibleActivityRegion();
            Set<ActivityNode, InterruptibleActivityRegion>& getNodes();
            Set<ActivityEdge, InterruptibleActivityRegion>& getInterruptingEdges();
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::INTERRUPTIBLE_ACTIVITY_REGION;
            }
    };
}

#endif