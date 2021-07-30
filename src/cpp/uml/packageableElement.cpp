#include "uml/packageableElement.h"
#include "uml/package.h"
#include "uml/universalFunctions.h"

using namespace UML;

PackageableElement::PackageableElement() {
    m_owningPackagePtr = 0;
}

PackageableElement::PackageableElement(const PackageableElement& el) : NamedElement(el) {
    m_owningPackageID = el.m_owningPackageID;
    m_owningPackagePtr = el.m_owningPackagePtr;
}

Package* PackageableElement::getOwningPackage() {
    return universalGet<Package>(m_owningPackageID, m_owningPackagePtr, m_manager);
}

void PackageableElement::setOwningPackage(Package* package) {
    if (!isSameOrNull(m_owningPackageID, package)) {
        if (!m_owningPackagePtr) {
            m_owningPackagePtr = &m_manager->get<Package>(m_owningPackageID);
        }
        if (m_owningPackagePtr->getPackagedElements().count(m_id)) {
            m_owningPackagePtr->getPackagedElements().remove(*this);
        }
        m_owningPackageID = ID::nullID();
        m_owningPackagePtr = 0;
    }

    if (package) {
        m_owningPackageID = package->getID();
    }

    if (!m_manager) {
        m_owningPackagePtr = package;
    }

    if (package) {
        if (!package->getPackagedElements().count(m_id)) {
            package->getPackagedElements().add(*this);
        }
    }
}

ElementType PackageableElement::getElementType() const {
    return ElementType::PACKAGEABLE_ELEMENT;
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