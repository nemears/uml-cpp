#ifndef _UML_OPAQUE_BEHAVIOR_H_
#define _UML_OPAQUE_BEHAVIOR_H_

#include "behavior.h"

namespace UML {

    class LiteralString;

    class OpaqueBehavior : public Behavior {

        template <typename AccessPolicy, typename SerializationPolicy, typename PersistencePolicy> friend class Manager;

        protected:
            CustomOrderedSet<LiteralString, OpaqueBehavior> m_bodies = CustomOrderedSet<LiteralString, OpaqueBehavior>(this);
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