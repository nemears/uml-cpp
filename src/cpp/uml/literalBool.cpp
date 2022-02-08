#include "uml/literalBool.h"
#include "uml/uml-stable.h"

using namespace UML;

LiteralBool::LiteralBool() : Element(ElementType::LITERAL_BOOL) {}

LiteralBool::LiteralBool(const LiteralBool& rhs) : Element(rhs, ElementType::LITERAL_BOOL) {}

bool LiteralBool::getValue() {
    return value;
}

void LiteralBool::setValue(bool val) {
    value = val;
}

bool LiteralBool::isSubClassOf(ElementType eType) const {
    bool ret = LiteralSpecification::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::LITERAL_BOOL;
    }

    return ret;
}