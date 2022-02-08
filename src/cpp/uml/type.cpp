#include "uml/type.h"
#include "uml/uml-stable.h"

using namespace UML;

Type::Type() : Element(ElementType::TYPE) {}

Type::Type(const Type& rhs) : Element(rhs, ElementType::TYPE) {}

bool Type::isSubClassOf(ElementType eType) const {
    bool ret = PackageableElement::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::TYPE;
    }

    return ret;
}