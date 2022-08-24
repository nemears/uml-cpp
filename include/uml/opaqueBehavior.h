#ifndef _UML_OPAQUE_BEHAVIOR_H_
#define _UML_OPAQUE_BEHAVIOR_H_

#include "behavior.h"

namespace UML {

    class LiteralString;

    class OpaqueBehavior : public Behavior {

        template <typename AccessPolicy, typename PersistencePolicy> friend class Manager;

        protected:
            OrderedSet<LiteralString, OpaqueBehavior> m_bodies = OrderedSet<LiteralString, OpaqueBehavior>(this);
            Set<LiteralString, OpaqueBehavior>& getBodiesSet();
            void init();
            OpaqueBehavior();
        public:
            virtual ~OpaqueBehavior();
            OrderedSet<LiteralString, OpaqueBehavior>& getBodies();
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::OPAQUE_BEHAVIOR;
            };
    };
}

#endif