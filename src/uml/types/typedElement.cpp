#include "uml/uml-stable.h"

using namespace UML;

void TypedElement::referenceErased(ID id) {
    NamedElement::referenceErased(id);
    eraseFromSet(id, m_type);
}

Singleton<Type, TypedElement>& TypedElement::getTypeSingleton() {
    return m_type;
}

TypedElement::TypedElement() : Element(ElementType::TYPED_ELEMENT) {
    
}

TypedElement::~TypedElement() {
    
}

TypePtr TypedElement::getType() const {
    return m_type.get();
}

void TypedElement::setType(Type* type) {
    m_type.set(type);
}

void TypedElement::setType(Type& type) {
    m_type.set(type);
}

void TypedElement::setType(ID id) {
    m_type.set(id);
}

bool TypedElement::is(ElementType eType) const {
    bool ret = NamedElement::is(eType);

    if (!ret) {
        ret = eType == ElementType::TYPED_ELEMENT;
    }

    return ret;
}
