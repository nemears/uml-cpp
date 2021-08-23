#include "uml/namespace.h"

using namespace UML;

void Namespace::AddMemberFunctor::operator()(NamedElement& el) const {
    if (!el.getMemberNamespace().count(m_el->getID())) {
        el.getMemberNamespace().add(*m_el);
    }

    m_el->getMembers().updateCopiedSequenceAddedTo<Namespace>(el, &Namespace::getMembers);
}

void Namespace::RemoveMemberFunctor::operator()(NamedElement& el) const {
    if (el.getMemberNamespace().count(m_el->getID())) {
        el.getMemberNamespace().remove(*m_el);
    }

    subsetsRemove<Namespace, NamedElement>(el, &Namespace::getOwnedMembers);

    m_el->getMembers().updateCopiedSequenceRemovedFrom<Namespace>(el, &Namespace::getMembers);
}

void Namespace::AddOwnedMemberFunctor::operator()(NamedElement& el) const {
    subsetsAdd<Element, Element>(el, &Element::getOwnedElements);
    subsetsAdd<Namespace, NamedElement>(el, &Namespace::getMembers);

    el.setNamespace(m_el);

    m_el->getOwnedMembers().updateCopiedSequenceAddedTo<Namespace>(el, &Namespace::getOwnedMembers);
}

void Namespace::RemoveOwnedMemberFunctor::operator()(NamedElement& el) const {
    subsetsRemove<Element, Element>(el, &Element::getOwnedElements);
    subsetsRemove<Namespace, NamedElement>(el, &Namespace::getMembers);

    if (el.getNamespace() == m_el) {
        el.setNamespace(0);
    }

    m_el->getOwnedMembers().updateCopiedSequenceRemovedFrom<Namespace>(el, &Namespace::getOwnedMembers);
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
    m_members.m_el = this;
    m_members.addProcedures.clear();
    m_members.addProcedures.push_back(new AddMemberFunctor(this));
    m_members.removeProcedures.clear();
    m_members.removeProcedures.push_back(new RemoveMemberFunctor(this));
    m_ownedMembers = nmspc.m_ownedMembers;
    m_ownedMembers.m_el = this;
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