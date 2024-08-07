#include "uml/uml-stable.h"

using namespace UML;

LiteralReal::LiteralReal() : Element(ElementType::LITERAL_REAL) {

}

double LiteralReal::getValue() {
    return m_value;
}

void LiteralReal::setValue(double val) {
    m_value = val;
}

bool LiteralReal::is(ElementType eType) const {
    bool ret = LiteralSpecification::is(eType);

    if (!ret) {
        ret = eType == ElementType::LITERAL_REAL;
    }

    return ret;
}
