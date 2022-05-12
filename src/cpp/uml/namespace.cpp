#include "uml/namespace.h"
#include "uml/setReferenceFunctor.h"
#include "uml/stereotype.h"
#include "uml/behavior.h"
#include "uml/dataType.h"
#include "uml/association.h"
#include "uml/deployment.h"
#include "uml/umlPtr.h"

using namespace UML;

void Namespace::AddElementImportFunctor::operator()(Element& el) const {
    if (el.as<ElementImport>().getImportedElement() && !m_el.as<Namespace>().getImportedMembers().contains(el.as<ElementImport>().getImportedElement().id())) {
        m_el.as<Namespace>().getImportedMembers().addReadOnly(*el.as<ElementImport>().getImportedElement());
    }
}

void Namespace::RemoveElementImportFunctor::operator()(Element& el) const {
    if (el.as<ElementImport>().getImportedElement() && m_el.as<Namespace>().getImportedMembers().contains(el.as<ElementImport>().getImportedElement().id())) {
        m_el.as<Namespace>().getImportedMembers().removeReadOnly(el.as<ElementImport>().getImportedElement().id());
    }
}

void Namespace::AddPackageImportFunctor::operator()(Element& el) const {
    if (el.as<PackageImport>().getImportedPackage()) {
        for (auto& pckgMember : el.as<PackageImport>().getImportedPackage()->getPackagedElements()) {
            if (!m_el.as<Namespace>().getImportedMembers().contains(pckgMember)) {
                m_el.as<Namespace>().getImportedMembers().addReadOnly(pckgMember);
            }
        }
    }
}

void Namespace::RemovePackageImportFunctor::operator()(Element& el) const {
    if (el.as<PackageImport>().getImportedPackage()) {
        for (auto& pckgMember : el.as<PackageImport>().getImportedPackage()->getPackagedElements()) {
            if (m_el.as<Namespace>().getImportedMembers().contains(pckgMember)) {
                m_el.as<Namespace>().getImportedMembers().removeReadOnly(pckgMember.getID());
            }
        }
    }
}

void Namespace::referenceReindexed(ID oldID, ID newID) {
    NamedElement::referenceReindexed(oldID, newID);
    m_members.reindex(oldID, newID);
}

void Namespace::reindexName(ID id, std::string newName) {
    NamedElement::reindexName(id, newName);
    m_members.reindexName(id, newName);
}

void Namespace::referencingReleased(ID id) {
    NamedElement::referencingReleased(id);
    m_members.release(id);
}

void Namespace::referenceErased(ID id) {
    NamedElement::referenceErased(id);
    m_members.eraseElement(id);
}

void Namespace::init() {
    m_members.m_readOnly = true;
    m_members.m_addFunctors.insert(new SetReferenceFunctor(this));
    m_members.m_removeFunctors.insert(new RemoveReferenceFunctor(this));
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
    m_elementImports.m_addFunctors.insert(new AddElementImportFunctor(this));
    m_elementImports.m_removeFunctors.insert(new RemoveElementImportFunctor(this));
    m_packageImports.subsets(*m_ownedElements);
    m_packageImports.opposite(&PackageImport::getImportingNamespaceSingleton);
    m_packageImports.m_addFunctors.insert(new AddPackageImportFunctor(this));
    m_packageImports.m_removeFunctors.insert(new RemovePackageImportFunctor(this));
}

Namespace::Namespace() : Element(ElementType::NAMESPACE) {
    init();
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