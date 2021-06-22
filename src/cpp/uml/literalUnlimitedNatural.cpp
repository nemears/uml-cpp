#include "uml/literalUnlimitedNatural.h"

using namespace UML;

LiteralUnlimitedNatural::LiteralUnlimitedNatural() {
    m_val = 0;
    m_infinite = false;
}

bool LiteralUnlimitedNatural::isInfinite() {
    return m_infinite;
}

unsigned long LiteralUnlimitedNatural::getNumberValue() {
    return m_val;
}

void LiteralUnlimitedNatural::setNumberValue(unsigned long val) {
    m_val = val;
}

void LiteralUnlimitedNatural::setInfinite() {
    m_val = 0;
    m_infinite = true;
}

ElementType LiteralUnlimitedNatural::getElementType() const {
    return ElementType::LITERAL_UNLIMITED_NATURAL;
}

bool LiteralUnlimitedNatural::isSubClassOf(ElementType eType) const {
    bool ret = LiteralSpecification::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::LITERAL_UNLIMITED_NATURAL;
    }

    return ret;
}