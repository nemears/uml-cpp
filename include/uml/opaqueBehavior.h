#ifndef OPAQUEBEHAVIORH
#define OPAQUEBEHAVIORH

#include "behavior.h"

namespace UML {

    class LiteralString;

    class OpaqueBehavior : public Behavior {

        friend class UmlManager;

        protected:
            OrderedSet<LiteralString, OpaqueBehavior> m_bodies = OrderedSet<LiteralString, OpaqueBehavior>(this); // TODO this should be a sequence according to spec
            void referencingReleased(ID id) override;
            void referenceReindexed(ID oldID, ID newID) override;
            void referenceErased(ID id) override;
            Set<LiteralString, OpaqueBehavior>& getBodiesSet();
            void init();
            void copy(const OpaqueBehavior& rhs);
            OpaqueBehavior();
        public:
            OpaqueBehavior(const OpaqueBehavior& rhs);
            virtual ~OpaqueBehavior();
            OrderedSet<LiteralString, OpaqueBehavior>& getBodies();
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::OPAQUE_BEHAVIOR;
            };
    };
}

#endif