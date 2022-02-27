#include "uml/packageableElement.h"
#include "uml/umlPtr.h"
#include "uml/package.h"

using namespace UML;

void PackageableElement::referenceReindexed(ID oldID, ID newID) {
    NamedElement::referenceReindexed(oldID, newID);
    // ParameterableElement::referenceReindexed(oldID, newID);
}

void PackageableElement::reindexName(std::string oldName, std::string newName) {
    NamedElement::reindexName(oldName, newName);
    // ParameterableElement::reindexName(oldName, newName);
}

void PackageableElement::referencingReleased(ID id) {
    NamedElement::referencingReleased(id);
    // ParameterableElement::referencingReleased(id);
}

void PackageableElement::restoreReference(Element* el) {
    NamedElement::restoreReference(el);
    // ParameterableElement::restoreReference(el);
}

void PackageableElement::referenceErased(ID id) {
    NamedElement::referenceErased(id);
    // ParameterableElement::referenceErased(id);
}

Set<Package, PackageableElement>& PackageableElement::getOwningPackageSingleton() {
    return m_owningPackage;
}

void PackageableElement::init() {
    m_owningPackage.subsets(m_namespace);
    m_owningPackage.opposite(&Package::getPackagedElements);
    m_owningPackage.m_signature = &PackageableElement::getOwningPackageSingleton;
}

PackageableElement::PackageableElement() : Element(ElementType::PACKAGEABLE_ELEMENT) {
    init();
}

PackagePtr PackageableElement::getOwningPackage() {
    PackagePtr ret(0);
    m_owningPackage.setPtr(ret);
    return ret;
}

void PackageableElement::setOwningPackage(Package* package) {
    m_owningPackage.set(package);
}

void PackageableElement::setOwningPackage(Package& package) {
    m_owningPackage.set(package);
}

void PackageableElement::setOwningPackage(ID id) {
    m_owningPackage.set(id);
}

bool PackageableElement::isSubClassOf(ElementType eType) const {
    bool ret = NamedElement::isSubClassOf(eType);

    // if (!ret) {
    //     ret = ParameterableElement::isSubClassOf(eType);
    // }

    if (!ret) {
        ret = eType == ElementType::PACKAGEABLE_ELEMENT;
    }

    return ret;
}