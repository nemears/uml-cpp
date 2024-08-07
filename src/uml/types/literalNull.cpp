#include "uml/uml-stable.h"

using namespace UML;

LiteralNull::LiteralNull() : Element(ElementType::LITERAL_NULL) {

}

bool LiteralNull::is(ElementType eType) const {
    bool ret = LiteralSpecification::is(eType);

    if (!ret) {
        ret = eType == ElementType::LITERAL_NULL;
    }

    return ret;
}
