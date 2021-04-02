#include "uml/literalReal.h"

using namespace UML;

double LiteralReal::getValue() {
    return value;
}

void LiteralReal::setValue(double val) {
    value = val;
}

ElementType LiteralReal::getElementType() {
    return ElementType::LITERAL_REAL;
}

bool LiteralReal::isSubClassOf(ElementType eType) {
    bool ret = ValueSpecification::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::LITERAL_REAL;
    }

    return ret;
}