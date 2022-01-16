#include "uml/package.h"
#include "uml/stereotype.h"
#include "uml/packageMerge.h"
#include "uml/profileApplication.h"
#include "uml/uml-stable.h"

using namespace UML;

void Package::referenceReindexed(ID oldID, ID newID) {
    Namespace::referenceReindexed(oldID, newID);
    PackageableElement::referenceReindexed(oldID, newID);
}

void Package::referencingReleased(ID id) {
    Namespace::referencingReleased(id);
    PackageableElement::referencingReleased(id);
}

void Package::referenceErased(ID id) {
    Namespace::referenceErased(id);
    PackageableElement::referenceErased(id);
}

void Package::init() {
    m_packagedElements.subsets(m_ownedMembers);
    m_packagedElements.opposite(&PackageableElement::getOwningPackageSingleton);
    m_packagedElements.m_signature = &Package::getPackagedElements;
    m_packageMerge.subsets(*m_ownedElements);
    m_packageMerge.opposite(&PackageMerge::getReceivingPackageSingleton);
    m_packageMerge.m_signature = &Package::getPackageMerge;
    m_ownedStereotypes.subsets(m_packagedElements);
    m_ownedStereotypes.m_signature = &Package::getOwnedStereotypes;
    m_profileApplications.subsets(*m_ownedElements);
    m_profileApplications.opposite(&ProfileApplication::getApplyingPackageSingleton);
    m_profileApplications.m_signature = &Package::getProfileApplications;
}

void Package::copy(const Package& rhs) {
    m_packagedElements = rhs.m_packagedElements;
    m_packageMerge = rhs.m_packageMerge;
    m_ownedStereotypes = rhs.m_ownedStereotypes;
}

Package::Package() : Element(ElementType::PACKAGE) {
    init();
}

Package::~Package() {
    
}

Package::Package(const Package& rhs) : Element(rhs, ElementType::PACKAGE) {
    init();
    Element::copy(rhs);
    NamedElement::copy(rhs);
    ParameterableElement::copy(rhs);
    PackageableElement::copy(rhs);
    Namespace::copy(rhs);
    TemplateableElement::copy(rhs);
    copy(rhs);
}

Set<PackageableElement, Package>& Package::getPackagedElements() {
    return m_packagedElements;
}

Set<PackageMerge, Package>& Package::getPackageMerge() {
    return m_packageMerge;
}

Set<ProfileApplication, Package>& Package::getProfileApplications() {
    return m_profileApplications;
}

Set<Stereotype, Package>& Package::getOwnedStereotypes() {
    return m_ownedStereotypes;
}

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