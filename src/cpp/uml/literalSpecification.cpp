#include "uml/literalSpecification.h"

using namespace UML;

LiteralSpecification::LiteralSpecification() : Element(ElementType::LITERAL_SPECIFICATION) {}

LiteralSpecification::LiteralSpecification(const LiteralSpecification& rhs) : Element(ElementType::LITERAL_SPECIFICATION) {}

bool LiteralSpecification::isSubClassOf(ElementType eType) const {
    bool ret = ValueSpecification::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::LITERAL_SPECIFICATION;
    }

    return ret;
}