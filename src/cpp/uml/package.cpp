#include "uml/package.h"
#include "uml/stereotype.h"
#include "uml/packageMerge.h"
#include "uml/profileApplication.h"

using namespace UML;

void Package::referenceReindexed(ID oldID, ID newID) {
    Namespace::referenceReindexed(oldID, newID);
    PackageableElement::referenceReindexed(oldID, newID);
    TemplateableElement::referenceReindexed(oldID, newID);
}

void Package::referencingReleased(ID id) {
    Namespace::referencingReleased(id);
    PackageableElement::referencingReleased(id);
    TemplateableElement::referencingReleased(id);
}

void Package::restoreReferences() {
    Namespace::restoreReferences();
    PackageableElement::restoreReferences();
    TemplateableElement::restoreReferences();
}

void Package::referenceErased(ID id) {
    Namespace::referenceErased(id);
    PackageableElement::referenceErased(id);
    TemplateableElement::referenceErased(id);
}

void Package::init() {
    m_packagedElements.subsets(m_ownedMembers);
    m_packagedElements.opposite(&PackageableElement::getOwningPackageSingleton);
    m_packagedElements.m_signature = &Package::getPackagedElements;
}

void Package::copy(const Package& rhs) {
    m_packagedElements = Set<PackageableElement, Package>(rhs.m_packagedElements);
    m_packagedElements.m_el = this;
}

Package::Package() : Element(ElementType::PACKAGE) {
    init();
    // m_packageMerge.subsets(*m_ownedElements);
    // m_packageMerge.subsets(*m_directedRelationships);
    // m_profileApplications.subsets(*m_ownedElements);
    // m_profileApplications.subsets(*m_directedRelationships);
    // m_ownedStereotypes.subsets(m_packagedElements);
}

Package::~Package() {
    
}

Package::Package(const Package& rhs) : Element(rhs, ElementType::PACKAGE) {
    NamedElement::copy(rhs);
    PackageableElement::copy(rhs);
    Namespace::copy(rhs);
    copy(rhs);
    NamedElement::init();
    PackageableElement::init();
    Namespace::init();
    init();
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