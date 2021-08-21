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
        if (m_manager) {
            m_manager->removeReference(m_id, m_owningPackageID);
        }
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
        if (m_manager) {
            m_manager->setReference(m_id, m_owningPackageID, this);
        }
        if (!package->getPackagedElements().count(m_id)) {
            package->getPackagedElements().add(*this);
        }
    }

    if (m_manager) {
        m_manager->updateCopiesSingleton<PackageableElement>(this, m_owningPackageID, &PackageableElement::m_owningPackageID);
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

void PackageableElement::restoreReleased(ID id, Element* released) {
    Element::restoreReleased(id, released);
}

void PackageableElement::referencingReleased(ID id) {
    Element::referencingReleased(id);
    if (m_owningPackageID == id) {
        m_owningPackagePtr = 0;
    }
}