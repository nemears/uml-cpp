#ifndef PRIMITIVETYPEH
#define PRIMITIVETYPEH

#include "dataType.h"

namespace UML{
    class PrimitiveType : public DataType {

        friend class UmlManager;

        protected:
            PrimitiveType();
        public:
            PrimitiveType(const PrimitiveType& rhs);
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::PRIMITIVE_TYPE;
            };
    };
}
#endif