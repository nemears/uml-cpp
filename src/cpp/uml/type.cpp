#include "uml/type.h"
#include "uml/package.h"

using namespace UML;

Type::Type() : Element(ElementType::TYPE) {

}

Type::~Type() {

}

bool Type::isSubClassOf(ElementType eType) const {
    bool ret = PackageableElement::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::TYPE;
    }

    return ret;
}