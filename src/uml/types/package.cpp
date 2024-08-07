#include "uml/uml-stable.h"

using namespace UML;

void Package::PackageableElementPolicy::elementAdded(PackageableElement& el, Package& me) {
    for (const UmlPtr<PackageImport>& import : packageImportsAdd) {
        if (import->getImportingNamespace()) {
            if (!import->getImportingNamespace()->getImportedMembers().contains(el.getID())) {
                import->getImportingNamespace()->m_importedMembers.innerAdd(ElementPtr(&el));
            }
        }
    }
    if (el.is(ElementType::STEREOTYPE) && !me.getOwnedStereotypes().contains(el.getID())) {
        me.m_ownedStereotypes.innerAdd(ElementPtr(&el));
    }
}

void Package::PackageableElementPolicy::elementRemoved(PackageableElement& el, Package& me) {
    for (const UmlPtr<PackageImport>& import : packageImportsRemove) {
        if (import->getImportingNamespace()) {
            if (import->getImportingNamespace()->getImportedMembers().contains(el.getID())) {
                import->getImportingNamespace()->m_importedMembers.innerRemove(ElementPtr(&el));
            }
        }
    }
    if (el.is(ElementType::STEREOTYPE) && me.getOwnedStereotypes().contains(el.getID())) {
        me.m_ownedStereotypes.innerRemove(ElementPtr(&el));
    }
}

void Package::referenceErased(ID id) {
    Namespace::referenceErased(id);
    PackageableElement::referenceErased(id);
}

// void Package::restoreReference(Element* el) {
//     Element::restoreReference(el);
//     if (el->isSubClassOf(ElementType::STEREOTYPE) && getPackagedElements().contains(el->getID()) && !getOwnedStereotypes().contains(el->getID())) {
//         m_ownedStereotypes.add(el->as<Stereotype>());
//     }
// }

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

Set<PackageableElement, Package, Package::PackageableElementPolicy>& Package::getPackagedElements() {
    return m_packagedElements;
}

Set<PackageMerge, Package>& Package::getPackageMerge() {
    return m_packageMerge;
}

Set<ProfileApplication, Package>& Package::getProfileApplications() {
    return m_profileApplications;
}

ReadOnlySet<Stereotype, Package>& Package::getOwnedStereotypes() {
    return m_ownedStereotypes;
}

bool Package::is(ElementType eType) const {
    bool ret = PackageableElement::is(eType);

    if (!ret) {
        ret = Namespace::is(eType);
    }

    if (!ret) {
        ret = TemplateableElement::is(eType);
    }

    if (!ret) {
        ret = eType == ElementType::PACKAGE;
    }

    return ret;
}
