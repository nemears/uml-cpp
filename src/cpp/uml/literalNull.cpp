#include "uml/literalNull.h"

using namespace UML;

ElementType LiteralNull::getElementType() const {
    return ElementType::LITERAL_NULL;
}

bool LiteralNull::isSubClassOf(ElementType eType) const {
    bool ret = LiteralSpecification::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::LITERAL_NULL;
    }

    return ret;
}