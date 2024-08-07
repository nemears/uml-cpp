#include "uml/uml-stable.h"

using namespace UML;

PrimitiveType::PrimitiveType() : Element(ElementType::PRIMITIVE_TYPE) {

}

bool PrimitiveType::is(ElementType eType) const {
    bool ret = DataType::is(eType);

    if (!ret) {
        ret = eType == ElementType::PRIMITIVE_TYPE;
    }

    return ret;
}
