#ifndef _UML_INTERRUPTIBLE_ACTIVITY_REGION_H_
#define _UML_INTERRUPTIBLE_ACTIVITY_REGION_H_

#include "activityGroup.h"

namespace UML {
    class InterruptibleActivityRegion : public ActivityGroup {

        template <typename AccessPolicy, typename PersistencePolicy> friend class Manager;

        protected:
            CustomSet<ActivityNode, InterruptibleActivityRegion> m_nodes = CustomSet<ActivityNode, InterruptibleActivityRegion>(this);
            CustomSet<ActivityEdge, InterruptibleActivityRegion> m_interruptingEdges = CustomSet<ActivityEdge, InterruptibleActivityRegion>(this);
            void referenceReindexed(ID newID) override;
            void referenceErased(ID id) override;
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