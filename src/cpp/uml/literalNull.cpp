#include "uml/literalNull.h"
#include "uml/uml-stable.h"

using namespace UML;

LiteralNull::LiteralNull() : Element(ElementType::LITERAL_NULL) {}

LiteralNull::LiteralNull(const LiteralNull& rhs) : Element(rhs, ElementType::LITERAL_NULL) {}

bool LiteralNull::isSubClassOf(ElementType eType) const {
    bool ret = LiteralSpecification::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::LITERAL_NULL;
    }

    return ret;
}