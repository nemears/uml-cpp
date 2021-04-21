#include "uml/namespace.h"

using namespace UML;

void Namespace::AddMemberFunctor::operator()(Element& el) const {
    dynamic_cast<NamedElement&>(el).setNamespace(dynamic_cast<Namespace*>(m_el));
    if (!el.getOwner()) {
        m_el->getOwnedElements().add(el);
    }
}

void Namespace::RemoveMemberFunctor::operator()(Element& el) const {
    if (dynamic_cast<NamedElement&>(el).getNamespace() == m_el) {
        dynamic_cast<NamedElement&>(el).setNamespace(0);
    }

    if (m_el->getOwnedElements().count(el.getID())) {
        m_el->getOwnedElements().remove(el);
    }
}

Namespace::Namespace() {
    m_members = new Sequence<NamedElement>;
    m_members->addProcedures.push_back(new AddMemberFunctor(this));
    m_members->removeProcedures.push_back(new RemoveMemberFunctor(this));
}

Namespace::~Namespace() {
    delete m_members;
}

Namespace::Namespace(const Namespace& nmspc) : NamedElement(nmspc), Element(nmspc) {
    m_members = new Sequence<NamedElement>(*nmspc.m_members);
    m_members->addProcedures.clear();
    m_members->addProcedures.push_back(new AddMemberFunctor(this));
    m_members->removeProcedures.clear();
    m_members->removeProcedures.push_back(new RemoveMemberFunctor(this));
}

Sequence<NamedElement>& Namespace::getMembers() {
    return *m_members;
}

ElementType Namespace::getElementType() {
    return ElementType::NAMESPACE;
}

bool Namespace::isSubClassOf(ElementType eType) {
    bool ret = NamedElement::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::NAMESPACE;
    }

    return ret;
}