#ifndef LITERALBOOLH
#define LITERALBOOLH

#include "literalSpecification.h"
#include "primitiveType.h"

namespace UML {
    class LiteralBool : public LiteralSpecification {
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
            ElementType getElementType() override;
            bool isSubClassOf(ElementType eType) override;
    };
}

#endif