#include "uml/literalUnlimitedNatural.h"

using namespace UML;

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