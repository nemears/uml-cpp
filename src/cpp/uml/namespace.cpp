#include "uml/namespace.h"

using namespace UML;

void Namespace::AddMemberFunctor::operator()(Element& el) const {
    dynamic_cast<NamedElement&>(el).setNamespace(dynamic_cast<Namespace*>(m_el));
    if (!el.getOwner()) {
        m_el->getOwnedElements().add(el);
    }
}

Namespace::Namespace() {
    m_members = new Sequence<NamedElement>;
    m_members->addProcedures.push_back(new AddMemberFunctor(this));
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