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
    if (m_owningPackage) {
        if (m_owningPackage->getPackagedElements().count(m_id)) {
            m_owningPackage->getPackagedElements().remove(*this);
        }
    }
    
    m_owningPackage = package;

    if (m_owningPackage) {
        if (!m_owningPackage->getPackagedElements().count(m_id)) {
            m_owningPackage->getPackagedElements().add(*this);
        }
    }
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