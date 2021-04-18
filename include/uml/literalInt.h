#ifndef LITERALINTH
#define LITERALINTH
#include "literalSpecification.h"
#include "primitiveType.h"

namespace UML {
    class LiteralInt : public LiteralSpecification {
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
            ElementType getElementType() override;
            bool isSubClassOf(ElementType eType) override;
    };
}

#endif