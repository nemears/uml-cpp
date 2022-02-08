#ifndef OPAQUEBEHAVIORH
#define OPAQUEBEHAVIORH

#include "behavior.h"

namespace UML {

    class LiteralString;

    class OpaqueBehavior : public Behavior {

        friend class UmlManager;

        protected:
            OrderedSet<LiteralString, OpaqueBehavior> m_bodies = OrderedSet<LiteralString, OpaqueBehavior>(this);
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