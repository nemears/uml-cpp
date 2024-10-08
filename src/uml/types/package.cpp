#include "uml/set/indexableSet.h"
#include "uml/types/element.h"
#include "uml/types/namedElement.h"
#include "uml/types/packageableElement.h"
#include "uml/types/parameterableElement.h"
#include "uml/types/templateableElement.h"
#include "uml/uml-stable.h"

using namespace UML;

void Package::PackageableElementPolicy::elementAdded(PackageableElement& el, Package& me) {
    for (const UmlPtr<PackageImport>& import : packageImportsAdd) {
        if (import->getImportingNamespace()) {
            if (!import->getImportingNamespace()->getImportedMembers().contains(el.getID())) {
                // import->getImportingNamespace()->m_importedMembers.innerAdd(ElementPtr(&el));
                me.addToReadonlySet(import->getImportingNamespace()->m_importedMembers, el);
            }
        }
    }
    if (el.is<Stereotype>() && !me.getOwnedStereotypes().contains(el.getID())) {
        me.m_ownedStereotypes.innerAdd(ElementPtr(&el));
    }
}

void Package::PackageableElementPolicy::elementRemoved(PackageableElement& el, Package& me) {
    for (const UmlPtr<PackageImport>& import : packageImportsRemove) {
        if (import->getImportingNamespace()) {
            if (import->getImportingNamespace()->getImportedMembers().contains(el.getID())) {
                me.removeFromReadonlySet(import->getImportingNamespace()->m_importedMembers, el);
            }
        }
    }
    if (el.is<Stereotype>() && me.getOwnedStereotypes().contains(el.getID())) {
        me.m_ownedStereotypes.innerRemove(ElementPtr(&el));
    }
}

Package::Package(std::size_t elementType, AbstractManager& manager) : 
    Element(elementType, manager),
    NamedElement(elementType, manager),
    ParameterableElement(elementType, manager),
    PackageableElement(elementType, manager),
    Namespace(elementType, manager), 
    TemplateableElement(elementType, manager)
{
    m_packagedElements.subsets(m_ownedMembers);
    m_packagedElements.opposite(&PackageableElement::getOwningPackageSingleton);
    m_packageMerge.subsets(m_ownedElements);
    m_packageMerge.opposite(&PackageMerge::getReceivingPackageSingleton);
    m_ownedStereotypes.subsets(m_packagedElements);
    m_profileApplications.subsets(m_ownedElements);
    m_profileApplications.opposite(&ProfileApplication::getApplyingPackageSingleton);
}

IndexableSet<PackageableElement, Package, Package::PackageableElementPolicy>& Package::getPackagedElements() {
    return m_packagedElements;
}

Set<PackageMerge, Package>& Package::getPackageMerge() {
    return m_packageMerge;
}

Set<ProfileApplication, Package>& Package::getProfileApplications() {
    return m_profileApplications;
}

ReadOnlyIndexableSet<Stereotype, Package>& Package::getOwnedStereotypes() {
    return m_ownedStereotypes;
}
