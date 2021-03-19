#include "uml/namespace.h"

using namespace UML;

Namespace::Namespace() {
    m_members = new Sequence<NamedElement>;
}

Namespace::~Namespace() {
    delete m_members;
}

Sequence<NamedElement>& Namespace::getMembers() {
    return *m_members;
}

ElementType Namespace::getElementType() {
    return ElementType::NAMESPACE;
}