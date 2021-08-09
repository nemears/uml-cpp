#include "uml/manifestation.h"
#include "uml/universalFunctions.h"
#include "uml/packageableElement.h"

using namespace UML;

Manifestation::Manifestation() {

}

Manifestation::Manifestation(const Manifestation& manifestation) {

}

Manifestation::~Manifestation() {

}

PackageableElement* Manifestation::getUtilizedElement() {
    return universalGet<PackageableElement>(m_utilizedElementID, m_utilizedElementPtr, m_manager);
}

void Manifestation::setUtilizedElement(PackageableElement* utilizedElement) {
    if (!isSameOrNull(m_utilizedElementID, utilizedElement)) {
        if (!m_utilizedElementPtr) {
            m_utilizedElementPtr = &m_manager->get<PackageableElement>(m_utilizedElementID);
        }

        if (m_supplier.count(m_utilizedElementID)) {
            m_supplier.remove(*m_utilizedElementPtr);
        }

        m_utilizedElementID = ID::nullID();
        m_utilizedElementPtr = 0;
    }

    if (utilizedElement) {
        m_utilizedElementID == utilizedElement->getID();
    }

    if (!m_manager) {
        m_utilizedElementPtr = utilizedElement;
    }

    if (utilizedElement) {
        if (!m_supplier.count(utilizedElement->getID())) {
            m_supplier.add(*utilizedElement);
        }
    }
}

ElementType Manifestation::getElementType() const {
    return ElementType::MANIFESTATION;
}

bool Manifestation::isSubClassOf(ElementType eType) const {
    bool ret = Abstraction::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::MANIFESTATION;
    }

    return ret;
}