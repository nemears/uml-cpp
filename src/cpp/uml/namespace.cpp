#include "uml/namespace.h"

using namespace UML;

// void Namespace::AddMemberFunctor::operator()(NamedElement& el) const {
//     oppositeSequenceAdd(el, &NamedElement::getMemberNamespace);
//     updateCopiedSequenceAddedTo(el, &Namespace::getMembers);
// }

// void Namespace::AddMemberFunctor::operator()(ID id) const {
//     // TODO update copies
// }

// void Namespace::RemoveMemberFunctor::operator()(NamedElement& el) const {
//     oppositeSequenceRemove(el, &NamedElement::getMemberNamespace);
//     subsetsRemove<Namespace, NamedElement>(el, &Namespace::getOwnedMembers);
//     el.updateQualifiedName("");
//     updateCopiedSequenceRemovedFrom(el, &Namespace::getMembers);
// }

// void Namespace::AddOwnedMemberFunctor::operator()(NamedElement& el) const {
//     //subsetsAdd<Element, Element>(el, &Element::getOwnedElements);
//     subsetsAdd<Namespace, NamedElement>(el, &Namespace::getMembers);
//     oppositeSingletonAdd(el, &NamedElement::setNamespace);
//     el.updateQualifiedName(m_el->getQualifiedName());
//     updateCopiedSequenceAddedTo(el, &Namespace::getOwnedMembers);
// }

// void Namespace::AddOwnedMemberFunctor::operator()(ID id) const {
//     if (!m_el->getOwnedElements().contains(id)) {
//         m_el->getOwnedElements().add(id);
//     }
//     if (!m_el->getMembers().count(id)) {
//         m_el->getMembers().addByID(id);
//     }
//     // TODO update copies
// }

// void Namespace::RemoveOwnedMemberFunctor::operator()(NamedElement& el) const {
//     //subsetsRemove<Element, Element>(el, &Element::getOwnedElements);
//     subsetsRemove<Namespace, NamedElement>(el, &Namespace::getMembers);
//     oppositeSingletonRemove(el, &NamedElement::m_namespace);
//     updateCopiedSequenceRemovedFrom(el, &Namespace::getOwnedMembers);
// }

void Namespace::referenceReindexed(ID oldID, ID newID) {
    NamedElement::referenceReindexed(oldID, newID);
    m_members.reindex(oldID, newID);
    m_ownedMembers.reindex(oldID, newID);
}

void Namespace::restoreReferences() {
    NamedElement::restoreReferences();
    // m_members.restoreReferences();
    // m_ownedMembers.restoreReferences();
}

void Namespace::referenceErased(ID id) {
    NamedElement::referenceErased(id);
    m_members.eraseElement(id);
    m_ownedMembers.eraseElement(id);
}

void Namespace::init() {
    m_ownedMembers.subsets(*m_ownedElements);
    m_members.opposite(&NamedElement::getNamespaceSingleton);
    m_members.subsets(m_ownedMembers);
}

void Namespace::copy(const Namespace& rhs) {
    m_members = Set<NamedElement, Namespace>(rhs.m_members);
    m_members.m_el = this;
    m_ownedMembers = Set<NamedElement, Namespace>(rhs.m_ownedMembers);
    m_ownedMembers.m_el = this;
}

Namespace::Namespace() : 
Element(ElementType::NAMESPACE) {
    init();
}

Namespace::~Namespace() {
    
}

Namespace::Namespace(const Namespace& nmspc) : Element(ElementType::NAMESPACE) {
    // abstract
}

void Namespace::setName(const std::string& name) {
    NamedElement::setName(name);
    for (auto& member : getOwnedMembers()) {
        member.updateQualifiedName(getQualifiedName());
    }
}

Set<NamedElement, Namespace>& Namespace::getMembers() {
    return m_members;
}

Set<NamedElement, Namespace>& Namespace::getOwnedMembers() {
    return m_ownedMembers;
}

bool Namespace::isSubClassOf(ElementType eType) const {
    bool ret = NamedElement::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::NAMESPACE;
    }

    return ret;
}

void Namespace::referencingReleased(ID id) {
    NamedElement::referencingReleased(id);
    m_ownedMembers.release(id);
    m_members.release(id);
}