#include "uml/uml-stable.h"

using namespace UML;

LiteralSpecification::LiteralSpecification() : Element(ElementType::LITERAL_SPECIFICATION) {

}

bool LiteralSpecification::is(ElementType eType) const {
    bool ret = ValueSpecification::is(eType);

    if (!ret) {
        ret = eType == ElementType::LITERAL_SPECIFICATION;
    }

    return ret;
}
