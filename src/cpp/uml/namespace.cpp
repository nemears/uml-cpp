#include "uml/namespace.h"

using namespace UML;

void Namespace::referenceReindexed(ID oldID, ID newID) {
    NamedElement::referenceReindexed(oldID, newID);
    m_members.reindex(oldID, newID);
}

void Namespace::referencingReleased(ID id) {
    NamedElement::referencingReleased(id);
    m_ownedMembers.release(id);
}

void Namespace::restoreReferences() {
    NamedElement::restoreReferences();
    // m_members.restoreReferences();
}

void Namespace::referenceErased(ID id) {
    NamedElement::referenceErased(id);
    m_members.eraseElement(id);
}

void Namespace::init() {
    m_members.opposite(&NamedElement::getMemberNamespace);
    m_members.m_signature = &Namespace::getMembers;
    m_members.m_readOnly = true;
    m_ownedMembers.subsets(*m_ownedElements);
    m_ownedMembers.subsets(m_members);
    m_ownedMembers.opposite(&NamedElement::getNamespaceSingleton);
    m_ownedMembers.m_signature = &Namespace::getOwnedMembers;
    m_ownedMembers.m_readOnly = true;
}

void Namespace::copy(const Namespace& rhs) {
    m_members = rhs.m_members;
    m_members.m_el = this;
    m_ownedMembers = rhs.m_ownedMembers;
    m_ownedMembers.m_el = this;
}

Namespace::Namespace() : Element(ElementType::NAMESPACE) {
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