#include "uml/package.h"
#include "uml/stereotype.h"
#include "uml/packageMerge.h"
#include "uml/profileApplication.h"

using namespace UML;

void Package::referenceReindexed(ID oldID, ID newID) {
    Namespace::referenceReindexed(oldID, newID);
    PackageableElement::referenceReindexed(oldID, newID);
    TemplateableElement::referenceReindexed(oldID, newID);
    m_packagedElements.reindex(oldID, newID);
    // m_packageMerge.reindex(oldID, newID);
    // m_profileApplications.reindex(oldID, newID);
    // m_ownedStereotypes.reindex(oldID, newID);
}

void Package::referencingReleased(ID id) {
    Namespace::referencingReleased(id);
    PackageableElement::referencingReleased(id);
    TemplateableElement::referencingReleased(id);
    m_packagedElements.release(id);
    // m_packageMerge.release(id);
    // m_profileApplications.release(id);
    // m_ownedStereotypes.release(id);
}

void Package::restoreReferences() {
    Namespace::restoreReferences();
    PackageableElement::restoreReferences();
    TemplateableElement::restoreReferences();
    // m_packagedElements.restoreReferences();
    // m_packageMerge.restoreReferences();
    // m_profileApplications.restoreReferences();
    // m_ownedStereotypes.restoreReferences();
}

void Package::referenceErased(ID id) {
    Namespace::referenceErased(id);
    PackageableElement::referenceErased(id);
    TemplateableElement::referenceErased(id);
    m_packagedElements.eraseElement(id);
    // m_packageMerge.eraseElement(id);
    // m_profileApplications.eraseElement(id);
    // m_ownedStereotypes.eraseElement(id);
}

Package::Package() : Element(ElementType::PACKAGE) {
    m_packagedElements.subsets(m_ownedMembers);
    m_packagedElements.opposite(&PackageableElement::getOwningPackageSingleton);
    // m_packageMerge.subsets(*m_ownedElements);
    // m_packageMerge.subsets(*m_directedRelationships);
    // m_profileApplications.subsets(*m_ownedElements);
    // m_profileApplications.subsets(*m_directedRelationships);
    // m_ownedStereotypes.subsets(m_packagedElements);
}

Package::~Package() {
    
}

Package::Package(const Package& pckg) : 
Namespace(pckg), 
PackageableElement(pckg), 
NamedElement(pckg), 
Element(pckg, ElementType::PACKAGE) {
    m_packagedElements = pckg.m_packagedElements;
    m_packagedElements.m_el = this;
    // m_packageMerge = pckg.m_packageMerge;
    // m_packageMerge.m_el = this;
    // m_profileApplications = pckg.m_profileApplications;
    // m_profileApplications.m_el = this;
    // m_ownedStereotypes = pckg.m_ownedStereotypes;
    // m_ownedStereotypes.m_el = this;
}

Set<PackageableElement, Package>& Package::getPackagedElements() {
    return m_packagedElements;
}

// Set<PackageMerge, Package>& Package::getPackageMerge() {
//     return m_packageMerge;
// }

// Set<ProfileApplication, Package>& Package::getProfileApplications() {
//     return m_profileApplications;
// }

// Set<Stereotype, Package>& Package::getOwnedStereotypes() {
//     return m_ownedStereotypes;
// }

bool Package::isSubClassOf(ElementType eType) const {
    bool ret = PackageableElement::isSubClassOf(eType);

    if (!ret) {
        ret = Namespace::isSubClassOf(eType);
    }

    if (!ret) {
        ret = TemplateableElement::isSubClassOf(eType);
    }

    if (!ret) {
        ret = eType == ElementType::PACKAGE;
    }

    return ret;
}