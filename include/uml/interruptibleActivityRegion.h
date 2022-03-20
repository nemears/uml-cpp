#ifndef _UML_INTERRUPTIBLE_ACTIVITY_REGION_H_
#define _UML_INTERRUPTIBLE_ACTIVITY_REGION_H_

#include "activityGroup.h"

namespace UML {
    class InterruptibleActivityRegion : public ActivityGroup {

        friend class UmlManager;

        protected:
            Set<ActivityNode, InterruptibleActivityRegion> m_nodes = Set<ActivityNode, InterruptibleActivityRegion>(this);
            Set<ActivityEdge, InterruptibleActivityRegion> m_interruptingEdges = Set<ActivityEdge, InterruptibleActivityRegion>(this);
            void referencingReleased(ID id) override;
            void referenceReindexed(ID oldID, ID newID) override;
            void reindexName(ID id, std::string newName) override;
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