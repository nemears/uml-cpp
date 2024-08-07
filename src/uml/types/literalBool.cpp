#include "uml/uml-stable.h"

using namespace UML;

LiteralBool::LiteralBool() : Element(ElementType::LITERAL_BOOL) {

}

bool LiteralBool::getValue() {
    return m_value;;
}

void LiteralBool::setValue(bool val) {
    m_value = val;
}

bool LiteralBool::is(ElementType eType) const {
    bool ret = LiteralSpecification::is(eType);

    if (!ret) {
        ret = eType == ElementType::LITERAL_BOOL;
    }

    return ret;
}
