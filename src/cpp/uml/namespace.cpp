#include "uml/namespace.h"
#include "uml/uml-stable.h"
#include "uml/setReferenceFunctor.h"

using namespace UML;

void Namespace::referenceReindexed(ID oldID, ID newID) {
    NamedElement::referenceReindexed(oldID, newID);
    m_members.reindex(oldID, newID);
    m_ownedMembers.reindex(oldID, newID);
}

void Namespace::referencingReleased(ID id) {
    NamedElement::referencingReleased(id);
    m_members.release(id);
    m_ownedMembers.release(id);
}

void Namespace::restoreReferences() {
    NamedElement::restoreReferences();
    // m_members.restoreReferences();
}

void Namespace::referenceErased(ID id) {
    NamedElement::referenceErased(id);
    m_members.eraseElement(id);
    m_ownedMembers.eraseElement(id);
}

void Namespace::init() {
    m_members.m_signature = &Namespace::getMembers;
    m_members.m_readOnly = true;
    m_members.m_addFunctors.insert(new SetReferenceFunctor(this));
    m_members.m_removeFunctors.insert(new RemoveReferenceFunctor(this));
    m_ownedMembers.subsets(*m_ownedElements);
    m_ownedMembers.subsets(m_members);
    m_ownedMembers.opposite(&NamedElement::getNamespaceSingleton);
    m_ownedMembers.m_signature = &Namespace::getOwnedMembers;
    m_ownedMembers.m_readOnly = true;
}

void Namespace::copy(const Namespace& rhs) {
    m_members = rhs.m_members;
    m_ownedMembers = rhs.m_ownedMembers;
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