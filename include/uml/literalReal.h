#ifndef LITERALREALH
#define LITERALREALH

#include "literalSpecification.h"
#include "primitiveType.h"

namespace UML {
    class LiteralReal : public LiteralSpecification {
        protected:
            double value;
        public:
            double getValue();
            void setValue(double val);
            ElementType getElementType() const override;
            bool isSubClassOf(ElementType eType) override;
            static ElementType elementType() {
                return ElementType::LITERAL_REAL;
            };
    };
}

#endif