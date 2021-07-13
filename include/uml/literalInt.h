#ifndef LITERALINTH
#define LITERALINTH

#include "literalSpecification.h"

namespace UML {
    class LiteralInt : public LiteralSpecification {
        protected:
            int value;
        public:
            int getValue();
            void setValue(int val);
            ElementType getElementType() const override;
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::LITERAL_INT;
            };
    };
}

#endif