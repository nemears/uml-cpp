#ifndef LITERALINTH
#define LITERALINTH
#include "valueSpecification.h"
#include "primitiveType.h"

namespace UML {
    class LiteralInt : public ValueSpecification {
        protected:
            int value;
        public:
            int getValue();
            void setValue(int val);
            LiteralInt() {
                PrimitiveType* intPrimitive = new PrimitiveType;
                intPrimitive->setPrimitiveType(PrimitiveType::Primitive::INT);
                setType(intPrimitive);
            }
    };
}

#endif