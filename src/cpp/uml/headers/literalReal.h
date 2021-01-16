#ifndef LITERALREAL
#define LITERALREAL
#include "valueSpecification.h"
#include "primitiveType.h"

namespace UML {
    class LiteralReal : public ValueSpecification {
        protected:
            double value;
        public:
            double getValue();
            void setValue(double val);
            LiteralReal() {
                PrimitiveType* realPrimitive = new PrimitiveType;
                realPrimitive->setPrimitiveType(PrimitiveType::Primitive::REAL);
                setType(realPrimitive);
            }
    };
}

#endif