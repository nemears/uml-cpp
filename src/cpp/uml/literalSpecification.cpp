#include "uml/literalSpecification.h"

using namespace UML;

ElementType LiteralSpecification::getElementType() const {
    return ElementType::LITERAL_SPECIFICATION;
}

bool LiteralSpecification::isSubClassOf(ElementType eType) const {
    bool ret = ValueSpecification::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::LITERAL_SPECIFICATION;
    }

    return ret;
}