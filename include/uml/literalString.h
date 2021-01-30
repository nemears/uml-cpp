#ifndef LITERALSTRINGH
#define LITERALSTRINGH
#include "valueSpecification.h"
#include "primitiveType.h"
#include <string>

namespace UML {
    class LiteralString : public ValueSpecification {
        protected:
            string value;
        public:
            string getValue();
            void setValue(string val);
            LiteralString() {
                PrimitiveType* stringPrimitive = new PrimitiveType;
                stringPrimitive->setPrimitiveType(PrimitiveType::Primitive::STRING);
                setType(stringPrimitive);
            }
            ElementType getElementType() override;
    };
}

#endif