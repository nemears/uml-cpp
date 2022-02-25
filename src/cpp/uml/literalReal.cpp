#include "uml/literalReal.h"
#include "uml/uml-stable.h"

using namespace UML;

LiteralReal::LiteralReal() : Element(ElementType::LITERAL_REAL) {}

LiteralReal::LiteralReal(const LiteralReal& rhs) : Element(rhs, ElementType::LITERAL_REAL) {
    m_value = rhs.m_value;
}

double LiteralReal::getValue() {
    return m_value;
}

void LiteralReal::setValue(double val) {
    m_value = val;
}

bool LiteralReal::isSubClassOf(ElementType eType) const {
    bool ret = LiteralSpecification::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::LITERAL_REAL;
    }

    return ret;
}