#include "uml/packageableElement.h"
#include "uml/package.h"

using namespace UML;

void PackageableElement::referenceReindexed(ID oldID, ID newID) {
    NamedElement::referenceReindexed(oldID, newID);
    ParameterableElement::referenceReindexed(oldID, newID);
    m_owningPackage.reindex(oldID, newID);
}

void PackageableElement::referencingReleased(ID id) {
    NamedElement::referencingReleased(id);
    ParameterableElement::referencingReleased(id);
    m_owningPackage.release(id);
}

void PackageableElement::restoreReferences() {
    NamedElement::restoreReferences();
    ParameterableElement::restoreReferences();
   // m_owningPackage.restoreReference();
}

void PackageableElement::referenceErased(ID id) {
    NamedElement::referenceErased(id);
    ParameterableElement::referenceErased(id);
    m_owningPackage.eraseElement(id);
}

Set<Package, PackageableElement>& PackageableElement::getOwningPackageSingleton() {
    return m_owningPackage;
}

PackageableElement::PackageableElement() : Element(ElementType::PACKAGEABLE_ELEMENT) {
    m_owningPackage.subsets(m_namespace);
    m_owningPackage.opposite(&Package::getPackagedElements);
}

PackageableElement::PackageableElement(const PackageableElement& el) : 
NamedElement(el), 
Element(el, ElementType::PACKAGEABLE_ELEMENT) {
    m_owningPackage = el.m_owningPackage;
    m_owningPackage.m_el = this;
}

Package* PackageableElement::getOwningPackage() {
    return m_owningPackage.get();
}

Package& PackageableElement::getOwningPackageRef() {
    return m_owningPackage.getRef();
}

ID PackageableElement::getOwningPackageID() const {
    return m_owningPackage.id();
}

bool PackageableElement::hasOwningPackage() const {
    return m_owningPackage.has();
}

void PackageableElement::setOwningPackage(Package* package) {
    m_owningPackage.set(package);
}

void PackageableElement::setOwningPackage(Package& package) {
    m_owningPackage.set(package);
}

bool PackageableElement::isSubClassOf(ElementType eType) const {
    bool ret = NamedElement::isSubClassOf(eType);

    if (!ret) {
        ret = ParameterableElement::isSubClassOf(eType);
    }

    if (!ret) {
        ret = eType == ElementType::PACKAGEABLE_ELEMENT;
    }

    return ret;
}