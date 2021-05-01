#include "uml/typedElement.h"

using namespace UML;

TypedElement::TypedElement(const TypedElement& el) {
    m_type = el.m_type;
}

Type* TypedElement::getType() {
    return m_type;
}

void TypedElement::setType(Type* type) {
    m_type = type;
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