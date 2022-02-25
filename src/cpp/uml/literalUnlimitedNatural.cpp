#include "uml/literalUnlimitedNatural.h"
#include "uml/uml-stable.h"

using namespace UML;

LiteralUnlimitedNatural::LiteralUnlimitedNatural() : Element(ElementType::LITERAL_UNLIMITED_NATURAL) {
    m_val = 0;
    m_infinite = false;
}

LiteralUnlimitedNatural::LiteralUnlimitedNatural(const LiteralUnlimitedNatural& rhs) : Element(rhs, ElementType::LITERAL_UNLIMITED_NATURAL) {
    m_val = rhs.m_val;
    m_infinite = rhs.m_infinite;
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

bool LiteralUnlimitedNatural::isSubClassOf(ElementType eType) const {
    bool ret = LiteralSpecification::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::LITERAL_UNLIMITED_NATURAL;
    }

    return ret;
}