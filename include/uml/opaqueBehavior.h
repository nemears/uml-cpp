#ifndef OPAQUEBEHAVIORH
#define OPAQUEBEHAVIORH

#include "behavior.h"

namespace UML {

    class LiteralString;

    class OpaqueBehavior : public Behavior {
        protected:
            Sequence<LiteralString> m_bodies;
        public:
            OpaqueBehavior();
            ~OpaqueBehavior();
            Sequence<LiteralString>& getBodies();
            ElementType getElementType() const override;
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::OPAQUE_BEHAVIOR;
            };
    };
}

#endif