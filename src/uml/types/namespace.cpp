#include "uml/uml-stable.h"

using namespace UML;

void Namespace::ElementImportPolicy::elementAdded(ElementImport& el, Namespace& me) {
    if (el.getImportedElement() && !me.getImportedMembers().contains(el.getImportedElement().id())) {
        me.m_importedMembers.innerAdd(el.getImportedElement());
    }
}

void Namespace::ElementImportPolicy::elementRemoved(ElementImport& el, Namespace& me) {
    if (el.getImportedElement() && me.getImportedMembers().contains(el.getImportedElement().id())) {
        me.m_importedMembers.innerRemove(el.getImportedElement());
    }
}

void Namespace::PackageImportPolicy::elementAdded(PackageImport& el, Namespace& me) {
    if (el.getImportedPackage()) {
        for (auto& pckgMember : el.getImportedPackage()->getPackagedElements()) {
            if (!me.getImportedMembers().contains(pckgMember)) {
                me.m_importedMembers.innerAdd(ElementPtr(&pckgMember));
            }
        }
    }
}

void Namespace::PackageImportPolicy::elementRemoved(PackageImport& el, Namespace& me) {
    if (el.getImportedPackage()) {
        for (auto& pckgMember : el.getImportedPackage()->getPackagedElements()) {
            if (me.getImportedMembers().contains(pckgMember)) {
                me.m_importedMembers.innerRemove(ElementPtr(&pckgMember));
            }
        }
    }
}

Namespace::Namespace(std::size_t elementType, AbstractManager& manager) : 
    Element(elementType, manager), 
    NamedElement(elementType, manager)  
{
    m_ownedMembers.subsets(m_ownedElements);
    m_ownedMembers.subsets(m_members);
    m_ownedMembers.opposite(&NamedElement::getNamespaceSingleton);
    m_ownedRules.subsets(m_ownedMembers);
    m_ownedRules.opposite(&Constraint::getContextSingleton);
    m_importedMembers.subsets(m_members);
    m_elementImports.subsets(m_ownedElements);
    m_elementImports.opposite(&ElementImport::getImportingNamespaceSingleton);
    m_packageImports.subsets(m_ownedElements);
    m_packageImports.opposite(&PackageImport::getImportingNamespaceSingleton);
}

void Namespace::setName(const std::string& name) {
    NamedElement::setName(name);
    for (auto& member : getOwnedMembers()) {
        member.updateQualifiedName(getQualifiedName());
    }
}

ReadOnlyIndexableSet<NamedElement, Namespace>& Namespace::getMembers() {
    return m_members;
}

ReadOnlyIndexableSet<NamedElement, Namespace>& Namespace::getOwnedMembers() {
    return m_ownedMembers;
}

Set<Constraint, Namespace>& Namespace::getOwnedRules() {
    return m_ownedRules;
}

ReadOnlyIndexableSet<PackageableElement, Namespace>& Namespace::getImportedMembers() {
    return m_importedMembers;
}

Set<ElementImport, Namespace, Namespace::ElementImportPolicy>& Namespace::getElementImports() {

    return m_elementImports;
}

Set<PackageImport, Namespace, Namespace::PackageImportPolicy>& Namespace::getPackageImports() {
    return m_packageImports;
}
