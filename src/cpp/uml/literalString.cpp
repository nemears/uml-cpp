#include "uml/literalString.h"

using namespace std;
using namespace UML;

string LiteralString::getValue() {
    return value;
}

void LiteralString::setValue(string val) {
    value = val;
}

ElementType LiteralString::getElementType() const {
    return ElementType::LITERAL_STRING;
}

bool LiteralString::isSubClassOf(ElementType eType) {
    bool ret = LiteralSpecification::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::LITERAL_STRING;
    }

    return ret;
}