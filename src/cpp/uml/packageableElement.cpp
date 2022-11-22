#include "uml/packageableElement.h"
#include "uml/umlPtr.h"
#include "uml/package.h"
#include "uml/stereotype.h"
#include "uml/behavior.h"
#include "uml/dataType.h"
#include "uml/association.h"
#include "uml/interface.h"
#include "uml/deployment.h"

using namespace UML;

void PackageableElement::referenceReindexed(ID oldID, ID newID) {
    NamedElement::referenceReindexed(oldID, newID);
    ParameterableElement::referenceReindexed(oldID, newID);
}

void PackageableElement::restoreReference(Element* el) {
    Element::restoreReference(el);
    ParameterableElement::restoreReference(el);
}

void PackageableElement::referenceErased(ID id) {
    NamedElement::referenceErased(id);
    ParameterableElement::referenceErased(id);
}

TypedSet<Package, PackageableElement>& PackageableElement::getOwningPackageSingleton() {
    return m_owningPackage;
}

void PackageableElement::init() {
    m_owningPackage.subsets(*m_namespace);
    m_owningPackage.opposite(&Package::getPackagedElements);
}

PackageableElement::PackageableElement() : Element(ElementType::PACKAGEABLE_ELEMENT) {
    init();
}

PackagePtr PackageableElement::getOwningPackage() const {
    return m_owningPackage.get();
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

    if (!ret) {
        ret = ParameterableElement::isSubClassOf(eType);
    }

    if (!ret) {
        ret = eType == ElementType::PACKAGEABLE_ELEMENT;
    }

    return ret;
}