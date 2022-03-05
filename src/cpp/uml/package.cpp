#include "uml/package.h"
#include "uml/stereotype.h"
#include "uml/behavior.h"
#include "uml/dataType.h"
#include "uml/association.h"
#include "uml/profileApplication.h"
#include "uml/interface.h"
#include "uml/deployment.h"

using namespace UML;

void Package::referenceReindexed(ID oldID, ID newID) {
    Namespace::referenceReindexed(oldID, newID);
    PackageableElement::referenceReindexed(oldID, newID);
}

void Package::reindexName(std::string oldName, std::string newName) {
    Namespace::reindexName(oldName, newName);
    PackageableElement::reindexName(oldName, newName);
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

Package::Package() : Element(ElementType::PACKAGE) {
    init();
}

Package::~Package() {
    mountAndRelease();
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