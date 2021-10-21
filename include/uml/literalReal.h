#ifndef LITERALREALH
#define LITERALREALH

#include "literalSpecification.h"

namespace UML {
    class LiteralReal : public LiteralSpecification {
        protected:
            double value = 0;
        public:
            double getValue();
            void setValue(double val);
            ElementType getElementType() const override;
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::LITERAL_REAL;
            };
    };
}

#endif