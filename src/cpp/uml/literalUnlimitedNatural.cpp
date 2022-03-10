#include "uml/literalUnlimitedNatural.h"
#include "uml/package.h"
#include "uml/behavior.h"
#include "uml/stereotype.h"
#include "uml/dataType.h"
#include "uml/association.h"
#include "uml/interface.h"
#include "uml/deployment.h"

using namespace UML;

LiteralUnlimitedNatural::LiteralUnlimitedNatural() : Element(ElementType::LITERAL_UNLIMITED_NATURAL) {
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

bool LiteralUnlimitedNatural::isSubClassOf(ElementType eType) const {
    bool ret = LiteralSpecification::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::LITERAL_UNLIMITED_NATURAL;
    }

    return ret;
}