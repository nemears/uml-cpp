#include "uml/package.h"

using namespace UML;

Package::Package() {
    m_ownedPackages = new Sequence<PackageableElement>();
}

Package::~Package() {
    delete m_ownedPackages;
}

Sequence<PackageableElement>& Package::getOwnedPackages() {
    return *m_ownedPackages;
}

ElementType Package::getElementType() {
    return ElementType::PACKAGE;
}

bool Package::isSubClassOf(ElementType eType) {
    bool ret = PackageableElement::isSubClassOf(eType);

    if (!ret) {
        ret = Namespace::isSubClassOf(eType);
    }

    if (!ret) {
        ret = eType == ElementType::PACKAGE;
    }

    return ret;
}