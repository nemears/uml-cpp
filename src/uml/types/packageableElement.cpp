#include "uml/uml-stable.h"

using namespace UML;

void PackageableElement::referenceErased(ID id) {
    NamedElement::referenceErased(id);
    ParameterableElement::referenceErased(id);
}

Singleton<Package, PackageableElement>& PackageableElement::getOwningPackageSingleton() {
    return m_owningPackage;
}

PackageableElement::PackageableElement() : Element(ElementType::PACKAGEABLE_ELEMENT) {
    m_owningPackage.subsets(m_namespace);
    m_owningPackage.opposite(&Package::getPackagedElements);
}

PackagePtr PackageableElement::getOwningPackage() const {
    return m_owningPackage.get();
}

void PackageableElement::setOwningPackage(PackagePtr package) {
    m_owningPackage.set(package);
}

void PackageableElement::setOwningPackage(Package& package) {
    m_owningPackage.set(package);
}

void PackageableElement::setOwningPackage(ID id) {
    m_owningPackage.set(id);
}

bool PackageableElement::is(ElementType eType) const {
    bool ret = NamedElement::is(eType);

    if (!ret) {
        ret = ParameterableElement::is(eType);
    }

    if (!ret) {
        ret = eType == ElementType::PACKAGEABLE_ELEMENT;
    }

    return ret;
}
