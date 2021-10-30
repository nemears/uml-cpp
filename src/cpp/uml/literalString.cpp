#include "uml/literalString.h"

using namespace std;
using namespace UML;

LiteralString::LiteralString() : Element(ElementType::LITERAL_STRING) {}

LiteralString::LiteralString(const LiteralString& rhs) : Element(rhs, ElementType::LITERAL_STRING) {}

string LiteralString::getValue() {
    return value;
}

void LiteralString::setValue(string val) {
    value = val;
}

bool LiteralString::isSubClassOf(ElementType eType) const {
    bool ret = LiteralSpecification::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::LITERAL_STRING;
    }

    return ret;
}