#include "uml/namespace.h"
#include "uml/stereotype.h"
#include "uml/behavior.h"
#include "uml/dataType.h"
#include "uml/association.h"
#include "uml/deployment.h"
#include "uml/umlPtr.h"

using namespace UML;

void Namespace::AddElementImportPolicy::apply(ElementImport& el, Namespace& me) {
    if (el.getImportedElement() && !me.getImportedMembers().contains(el.getImportedElement().id())) {
        me.m_importedMembers.innerAdd(*el.getImportedElement());
    }
}

void Namespace::RemoveElementImportPolicy::apply(ElementImport& el, Namespace& me) {
    if (el.getImportedElement() && me.getImportedMembers().contains(el.getImportedElement().id())) {
        me.m_importedMembers.innerRemove(el.getImportedElement().id());
    }
}

void Namespace::AddPackageImportPolicy::apply(PackageImport& el, Namespace& me) {
    if (el.getImportedPackage()) {
        for (auto& pckgMember : el.getImportedPackage()->getPackagedElements()) {
            if (!me.getImportedMembers().contains(pckgMember)) {
                me.m_importedMembers.innerAdd(pckgMember);
            }
        }
    }
}

void Namespace::RemovePackageImportPolicy::apply(PackageImport& el, Namespace& me) {
    if (el.getImportedPackage()) {
        for (auto& pckgMember : el.getImportedPackage()->getPackagedElements()) {
            if (me.getImportedMembers().contains(pckgMember)) {
                me.m_importedMembers.innerRemove(pckgMember.getID());
            }
        }
    }
}

void Namespace::referenceReindexed(ID newID) {
    NamedElement::referenceReindexed(newID);
    m_members.reindex(newID);
}

void Namespace::referenceErased(ID id) {
    NamedElement::referenceErased(id);
    m_members.eraseElement(id);
}

Namespace::Namespace() : Element(ElementType::NAMESPACE) {
    m_members.m_readOnly = true;
    m_ownedMembers.subsets(*m_ownedElements);
    m_ownedMembers.subsets(m_members);
    m_ownedMembers.opposite(&NamedElement::getNamespaceSingleton);
    m_ownedMembers.m_readOnly = true;
    m_ownedRules.subsets(m_ownedMembers);
    m_ownedRules.opposite(&Constraint::getContextSingleton);
    m_importedMembers.subsets(m_members);
    m_importedMembers.m_readOnly = true;
    m_elementImports.subsets(*m_ownedElements);
    m_elementImports.opposite(&ElementImport::getImportingNamespaceSingleton);
    m_packageImports.subsets(*m_ownedElements);
    m_packageImports.opposite(&PackageImport::getImportingNamespaceSingleton);
}

Namespace::~Namespace() {
    
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

Set<Constraint, Namespace>& Namespace::getOwnedRules() {
    return m_ownedRules;
}

Set<PackageableElement, Namespace>& Namespace::getImportedMembers() {
    return m_importedMembers;
}

Set<ElementImport, Namespace>& Namespace::getElementImports() {
    return m_elementImports;
}

Set<PackageImport, Namespace>& Namespace::getPackageImports() {
    return m_packageImports;
}

bool Namespace::isSubClassOf(ElementType eType) const {
    bool ret = NamedElement::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::NAMESPACE;
    }

    return ret;
}