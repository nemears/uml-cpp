#include "uml/uml-stable.h"

using namespace UML;

Type::Type() : Element(ElementType::TYPE) {

}

Type::~Type() {

}

bool Type::is(ElementType eType) const {
    bool ret = PackageableElement::is(eType);

    if (!ret) {
        ret = eType == ElementType::TYPE;
    }

    return ret;
}
