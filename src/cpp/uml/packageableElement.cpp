#include "uml/packageableElement.h"
#include "uml/package.h"

using namespace UML;

PackageableElement::PackageableElement() {
    m_owningPackage = 0;
}

Package* PackageableElement::getOwningPackage() {
    return m_owningPackage;
}

void PackageableElement::setOwningPackage(Package* package) {
    m_owningPackage = package;
}

ElementType PackageableElement::getElementType() {
    return ElementType::PACKAGEABLE_ELEMENT;
}

bool PackageableElement::isSubClassOf(ElementType eType) {
    bool ret = NamedElement::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::PACKAGEABLE_ELEMENT;
    }

    return ret;
}