#include "uml/uml-stable.h"

using namespace UML;

LiteralInt::LiteralInt() : Element(ElementType::LITERAL_INT) {

}

int LiteralInt::getValue() {
    return m_value;
}

void LiteralInt::setValue(int val) {
    m_value = val;
}

bool LiteralInt::is(ElementType eType) const {
    bool ret = LiteralSpecification::is(eType);

    if (!ret) {
        ret = eType == ElementType::LITERAL_INT;
    }

    return ret;
}
