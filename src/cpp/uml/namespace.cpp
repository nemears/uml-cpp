#include "uml/namespace.h"

using namespace UML;

void Namespace::AddMemberFunctor::operator()(Element& el) const {
    dynamic_cast<NamedElement&>(el).getMemberNamespace().add(*dynamic_cast<Namespace*>(m_el));
}

void Namespace::RemoveMemberFunctor::operator()(Element& el) const {
    if (dynamic_cast<NamedElement&>(el).getMemberNamespace().count(m_el->getID())) {
        dynamic_cast<NamedElement&>(el).getMemberNamespace().remove(*dynamic_cast<Namespace*>(m_el));
    }

    if (dynamic_cast<Namespace*>(m_el)->getOwnedMembers().count(el.getID())) {
        dynamic_cast<Namespace*>(m_el)->getOwnedMembers().remove(dynamic_cast<NamedElement&>(el));
    }
}

void Namespace::AddOwnedMemberFunctor::operator()(Element& el) const {
    dynamic_cast<NamedElement&>(el).setNamespace(dynamic_cast<Namespace*>(m_el));

    if (!dynamic_cast<Namespace*>(m_el)->getMembers().count(el.getID())) {
        dynamic_cast<Namespace*>(m_el)->getMembers().add(dynamic_cast<NamedElement&>(el));
    }

    if (!el.getOwner()) {
        m_el->getOwnedElements().add(el);
    }
}

void Namespace::RemoveOwnedMemberFunctor::operator()(Element& el) const {
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
    m_ownedMembers = new Sequence<NamedElement>;
    m_ownedMembers->addProcedures.push_back(new AddOwnedMemberFunctor(this));
    m_ownedMembers->removeProcedures.push_back(new RemoveOwnedMemberFunctor(this));
}

Namespace::~Namespace() {
    delete m_members;
    delete m_ownedMembers;
}

Namespace::Namespace(const Namespace& nmspc) : NamedElement(nmspc), Element(nmspc) {
    m_members = new Sequence<NamedElement>(*nmspc.m_members);
    m_members->addProcedures.clear();
    m_members->addProcedures.push_back(new AddMemberFunctor(this));
    m_members->removeProcedures.clear();
    m_members->removeProcedures.push_back(new RemoveMemberFunctor(this));
    m_ownedMembers = new Sequence<NamedElement>(*nmspc.m_ownedMembers);
    m_ownedMembers->addProcedures.clear();
    m_ownedMembers->addProcedures.push_back(new AddOwnedMemberFunctor(this));
    m_ownedMembers->removeProcedures.clear();
    m_ownedMembers->removeProcedures.push_back(new RemoveOwnedMemberFunctor(this));
}

Sequence<NamedElement>& Namespace::getMembers() {
    return *m_members;
}

Sequence<NamedElement>& Namespace::getOwnedMembers() {
    return *m_ownedMembers;
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