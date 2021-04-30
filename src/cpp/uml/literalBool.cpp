#include "uml/literalBool.h"

using namespace UML;

bool LiteralBool::getValue() {
    return value;
}

void LiteralBool::setValue(bool val) {
    value = val;
}

ElementType LiteralBool::getElementType() const {
    return ElementType::LITERAL_BOOL;
}

bool LiteralBool::isSubClassOf(ElementType eType) {
    bool ret = LiteralSpecification::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::LITERAL_BOOL;
    }

    return ret;
}