#include "uml/types/namedElement.h"
#include "uml/uml-stable.h"

using namespace UML;

Singleton<Type, TypedElement>& TypedElement::getTypeSingleton() {
    return m_type;
}

TypedElement::TypedElement(std::size_t elementType, AbstractManager& manager) : 
    Element(elementType, manager),
    NamedElement(elementType, manager) 
{}
    
TypePtr TypedElement::getType() const {
    return m_type.get();
}

void TypedElement::setType(TypePtr type) {
    m_type.set(type);
}

void TypedElement::setType(Type& type) {
    m_type.set(type);
}

void TypedElement::setType(ID id) {
    m_type.set(id);
}
