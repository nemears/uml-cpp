#include "uml/literalSpecification.h"

using namespace UML;

ElementType LiteralSpecification::getElementType() {
    return ElementType::LITERAL_SPECIFICATION;
}

bool LiteralSpecification::isSubClassOf(ElementType eType) {
    bool ret = ValueSpecification::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::LITERAL_SPECIFICATION;
    }

    return ret;
}