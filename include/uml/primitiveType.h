#ifndef PRIMITIVETYPEH
#define PRIMITIVETYPEH

#include "dataType.h"

namespace UML{
class PrimitiveType : public DataType {
    public:
        ElementType getElementType() override;
        bool isSubClassOf(ElementType eType) override;
};
}
#endif