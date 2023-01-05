#include "uml/package.h"
#include "uml/stereotype.h"
#include "uml/behavior.h"
#include "uml/dataType.h"
#include "uml/association.h"
#include "uml/profileApplication.h"
#include "uml/interface.h"
#include "uml/deployment.h"

using namespace UML;

void Package::AddPackageableElementPolicy::apply(PackageableElement& el, __attribute__((unused)) Package& me) {
    for (const UmlPtr<PackageImport>& import : packageImportsAdd) {
        if (import->getImportingNamespace()) {
            if (!import->getImportingNamespace()->getImportedMembers().contains(el.getID())) {
                import->getImportingNamespace()->getImportedMembers().addReadOnly(el);
            }
        }
    }
}

void Package::RemovePackageableElementPolicy::apply(PackageableElement& el, __attribute__((unused)) Package& me) {
    for (const UmlPtr<PackageImport>& import : packageImportsRemove) {
        if (import->getImportingNamespace()) {
            if (import->getImportingNamespace()->getImportedMembers().contains(el.getID())) {
                import->getImportingNamespace()->getImportedMembers().removeReadOnly(el.getID());
            }
        }
    }
}

void Package::referenceReindexed(ID newID) {
    Namespace::referenceReindexed(newID);
    PackageableElement::referenceReindexed(newID);
}

void Package::referenceErased(ID id) {
    Namespace::referenceErased(id);
    PackageableElement::referenceErased(id);
}

Package::Package() : Element(ElementType::PACKAGE) {
    m_packagedElements.subsets(m_ownedMembers);
    m_packagedElements.opposite(&PackageableElement::getOwningPackageSingleton);
    m_packageMerge.subsets(*m_ownedElements);
    m_packageMerge.opposite(&PackageMerge::getReceivingPackageSingleton);
    m_ownedStereotypes.subsets(m_packagedElements);
    m_profileApplications.subsets(*m_ownedElements);
    m_profileApplications.opposite(&ProfileApplication::getApplyingPackageSingleton);
}

Package::~Package() {
    
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