#include "uml/types/packageableElement.h"
#include "uml/umlPtr.h"
#include "uml/types/package.h"

using namespace UML;

void PackageableElement::referenceErased(ID id) {
    NamedElement::referenceErased(id);
    // ParameterableElement::referenceErased(id);
}

Singleton<Package, PackageableElement>& PackageableElement::getOwningPackageSingleton() {
    return m_owningPackage;
}

PackageableElement::PackageableElement() : Element(ElementType::PACKAGEABLE_ELEMENT) {
    m_owningPackage.subsets(*m_namespace);
    m_owningPackage.opposite(&Package::getPackagedElements);
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

    // if (!ret) {
    //     ret = ParameterableElement::isSubClassOf(eType);
    // }

    if (!ret) {
        ret = eType == ElementType::PACKAGEABLE_ELEMENT;
    }

    return ret;
}
