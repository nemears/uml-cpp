#include "uml/literalInt.h"

using namespace UML;

LiteralInt::LiteralInt() : Element(ElementType::LITERAL_INT) {}

LiteralInt::LiteralInt(const LiteralInt& rhs) : Element(rhs, ElementType::LITERAL_INT) {}

int LiteralInt::getValue() {
    return value;
}

void LiteralInt::setValue(int val) {
    value = val;
}

bool LiteralInt::isSubClassOf(ElementType eType) const {
    bool ret = LiteralSpecification::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::LITERAL_INT;
    }

    return ret;
}