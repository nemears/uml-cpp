#ifndef LITERALSTRING
#define LITERALSTRING
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
    };
}

#endif