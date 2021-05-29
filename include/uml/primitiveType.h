#ifndef PRIMITIVETYPEH
#define PRIMITIVETYPEH

#include "dataType.h"

namespace UML{
    class PrimitiveType : public DataType {
        public:
            ElementType getElementType() const override;
            bool isSubClassOf(ElementType eType) override;
            static ElementType elementType() {
                return ElementType::PRIMITIVE_TYPE;
            };
    };
}
#endif