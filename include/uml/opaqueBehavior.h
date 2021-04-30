#ifndef OPAQUEBEHAVIORH
#define OPAQUEBEHAVIORH

#include "literalString.h"
#include "behavior.h"

namespace UML {
    class OpaqueBehavior : public Behavior {
        protected:
            Sequence<LiteralString>* m_bodies;
        public:
            OpaqueBehavior();
            ~OpaqueBehavior();
            Sequence<LiteralString>& getBodies();
            ElementType getElementType() const override;
            bool isSubClassOf(ElementType eType) override;
    };
}

#endif