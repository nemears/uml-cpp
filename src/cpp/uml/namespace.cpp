#include "uml/namespace.h"

using namespace UML;

void Namespace::AddMemberFunctor::operator()(Element& el) const {
    if (!dynamic_cast<NamedElement&>(el).getMemberNamespace().count(m_el->getID())) {
        dynamic_cast<NamedElement&>(el).getMemberNamespace().add(*dynamic_cast<Namespace*>(m_el));
    }
}

void Namespace::RemoveMemberFunctor::operator()(Element& el) const {
    if (dynamic_cast<NamedElement&>(el).getMemberNamespace().count(m_el->getID())) {
        dynamic_cast<NamedElement&>(el).getMemberNamespace().remove(*dynamic_cast<Namespace*>(m_el));
    }

    if (dynamic_cast<Namespace*>(m_el)->getOwnedMembers().count(el.getID())) {
        dynamic_cast<Namespace*>(m_el)->getOwnedMembers().internalRemove(dynamic_cast<NamedElement&>(el));
    }
}

void Namespace::AddOwnedMemberFunctor::operator()(Element& el) const {
    dynamic_cast<NamedElement&>(el).setNamespace(dynamic_cast<Namespace*>(m_el));

    if (!dynamic_cast<Namespace*>(m_el)->getMembers().count(el.getID())) {
        dynamic_cast<Namespace*>(m_el)->getMembers().add(dynamic_cast<NamedElement&>(el));
    }

    if (!m_el->getOwnedElements().count(el.getID())) {
        m_el->getOwnedElements().internalAdd(el);
    }
}

void Namespace::RemoveOwnedMemberFunctor::operator()(Element& el) const {
    if (dynamic_cast<NamedElement&>(el).getNamespace() == m_el) {
        dynamic_cast<NamedElement&>(el).setNamespace(0);
    }

    if (m_el->getOwnedElements().count(el.getID())) {
        m_el->getOwnedElements().internalRemove(el);
    }
}

void Namespace::setManager(UmlManager* manager) {
    NamedElement::setManager(manager);
    m_members.m_manager = manager;
    m_ownedMembers.m_manager = manager;
}

Namespace::Namespace() : NamedElement() {
    m_members.addProcedures.push_back(new AddMemberFunctor(this));
    m_members.removeProcedures.push_back(new RemoveMemberFunctor(this));
    m_ownedMembers.addProcedures.push_back(new AddOwnedMemberFunctor(this));
    m_ownedMembers.removeProcedures.push_back(new RemoveOwnedMemberFunctor(this));
}

Namespace::~Namespace() {
    
}

Namespace::Namespace(const Namespace& nmspc) : NamedElement(nmspc), Element(nmspc) {
    m_members = nmspc.m_members;
    m_members.addProcedures.clear();
    m_members.addProcedures.push_back(new AddMemberFunctor(this));
    m_members.removeProcedures.clear();
    m_members.removeProcedures.push_back(new RemoveMemberFunctor(this));
    m_ownedMembers = nmspc.m_ownedMembers;
    m_ownedMembers.addProcedures.clear();
    m_ownedMembers.addProcedures.push_back(new AddOwnedMemberFunctor(this));
    m_ownedMembers.removeProcedures.clear();
    m_ownedMembers.removeProcedures.push_back(new RemoveOwnedMemberFunctor(this));
}

Sequence<NamedElement>& Namespace::getMembers() {
    return m_members;
}

Sequence<NamedElement>& Namespace::getOwnedMembers() {
    return m_ownedMembers;
}

ElementType Namespace::getElementType() const {
    return ElementType::NAMESPACE;
}

bool Namespace::isSubClassOf(ElementType eType) const {
    bool ret = NamedElement::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::NAMESPACE;
    }

    return ret;
}

void Namespace::restoreReleased(ID id, Element* released) {
    NamedElement::restoreReleased(id, released);
    if (m_ownedMembers.count(id)) {
        released->as<NamedElement>().setNamespace(this);
    }
    if (m_members.count(id)) {
        if (!released->as<NamedElement>().m_memberNamespace->count(m_id)) {
            released->as<NamedElement>().m_memberNamespace->add(*this);
        }
    }
}

void Namespace::referencingReleased(ID id) {
    NamedElement::referencingReleased(id);
    m_ownedMembers.elementReleased(id);
    m_members.elementReleased(id);
}