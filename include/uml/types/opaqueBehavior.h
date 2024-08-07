#pragma once

#include "behavior.h"

namespace UML {

    class LiteralString;

    class OpaqueBehavior : public Behavior {

        template <typename SerializationPolicy, typename PersistencePolicy> friend class Manager;

        protected:
            OrderedSet<LiteralString, OpaqueBehavior> m_bodies = OrderedSet<LiteralString, OpaqueBehavior>(this);
            OpaqueBehavior();
        public:
            virtual ~OpaqueBehavior();
            OrderedSet<LiteralString, OpaqueBehavior>& getBodies();
            bool is(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::OPAQUE_BEHAVIOR;
            };
    };
}
