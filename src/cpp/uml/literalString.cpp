#include "uml/literalString.h"
#include "uml/uml-stable.h"

using namespace UML;

LiteralString::LiteralString() : Element(ElementType::LITERAL_STRING) {}

LiteralString::LiteralString(const LiteralString& rhs) : Element(rhs, ElementType::LITERAL_STRING) {}

std::string LiteralString::getValue() {
    return value;
}

void LiteralString::setValue(std::string val) {
    value = val;
}

bool LiteralString::isSubClassOf(ElementType eType) const {
    bool ret = LiteralSpecification::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::LITERAL_STRING;
    }

    return ret;
}