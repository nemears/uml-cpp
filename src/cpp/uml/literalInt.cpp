#include "uml/literalInt.h"
#include "uml/uml-stable.h"

using namespace UML;

LiteralInt::LiteralInt() : Element(ElementType::LITERAL_INT) {}

LiteralInt::LiteralInt(const LiteralInt& rhs) : Element(rhs, ElementType::LITERAL_INT) {
    m_value = rhs.m_value;
}

int LiteralInt::getValue() {
    return m_value;
}

void LiteralInt::setValue(int val) {
    m_value = val;
}

bool LiteralInt::isSubClassOf(ElementType eType) const {
    bool ret = LiteralSpecification::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::LITERAL_INT;
    }

    return ret;
}