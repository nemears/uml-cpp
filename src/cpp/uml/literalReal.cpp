#include "uml/literalReal.h"
#include "uml/uml-stable.h"

using namespace UML;

LiteralReal::LiteralReal() : Element(ElementType::LITERAL_REAL) {}

LiteralReal::LiteralReal(const LiteralReal& rhs) : Element(rhs, ElementType::LITERAL_REAL) {}

double LiteralReal::getValue() {
    return value;
}

void LiteralReal::setValue(double val) {
    value = val;
}

bool LiteralReal::isSubClassOf(ElementType eType) const {
    bool ret = LiteralSpecification::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::LITERAL_REAL;
    }

    return ret;
}