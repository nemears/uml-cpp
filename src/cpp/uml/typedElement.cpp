#include "uml/typedElement.h"

using namespace UML;

TypedElement::TypedElement(const TypedElement& el) {
    type = el.type;
}

Type* TypedElement::getType() {
    return type;
}

void TypedElement::setType(Type* type) {
    this->type = type;
}

ElementType TypedElement::getElementType() const {
    return ElementType::TYPED_ELEMENT;
}

bool TypedElement::isSubClassOf(ElementType eType) {
    bool ret = NamedElement::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::TYPED_ELEMENT;
    }

    return ret;
}