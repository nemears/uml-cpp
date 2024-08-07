#include "uml/uml-stable.h"

using namespace UML;

LiteralString::LiteralString() : Element(ElementType::LITERAL_STRING) {

}

std::string LiteralString::getValue() {
    return m_value;
}

void LiteralString::setValue(const std::string& val) {
    m_value = val;
}

bool LiteralString::is(ElementType eType) const {
    bool ret = LiteralSpecification::is(eType);

    if (!ret) {
        ret = eType == ElementType::LITERAL_STRING;
    }

    return ret;
}
