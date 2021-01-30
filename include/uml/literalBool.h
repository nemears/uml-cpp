#ifndef LITERALBOOLH
#define LITERALBOOLH
#include "valueSpecification.h"
#include "primitiveType.h"

namespace UML {
    class LiteralBool : public ValueSpecification {
        protected:
            bool value;
        public:
            bool getValue();
            void setValue(bool val);
            LiteralBool() {
                PrimitiveType* boolPrimitive = new PrimitiveType;
                boolPrimitive->setPrimitiveType(PrimitiveType::Primitive::BOOL);
                setType(boolPrimitive);
            }
    };
}

#endif