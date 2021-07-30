#include "uml/enumerationLiteral.h"
#include "uml/enumeration.h"
#include "uml/universalFunctions.h"

using namespace UML;

EnumerationLiteral::EnumerationLiteral() {
    m_enumerationPtr = 0;
}

Enumeration* EnumerationLiteral::getEnumeration() {
    return universalGet<Enumeration>(m_enumerationID, m_enumerationPtr, m_manager);
}

void EnumerationLiteral::setEnumeration(Enumeration* enumeration) {
    if (!m_enumerationID.isNull()) {
        if (!m_enumerationPtr) {
            m_enumerationPtr = &m_manager->get<Enumeration>(m_enumerationID);
        }
        if (m_enumerationPtr->getOwnedLiteral().count(m_id)) {
            m_enumerationPtr->getOwnedLiteral().remove(*this);
        }
        m_enumerationID = ID::nullID();
        m_enumerationPtr = 0;
    }

    if (enumeration) {
        m_enumerationID = enumeration->getID();
    }

    if (!m_manager) {
        m_enumerationPtr = enumeration;
    }

    if (enumeration) {
        if (!enumeration->getOwnedLiteral().count(m_id)) {
            enumeration->getOwnedLiteral().add(*this);
        }
    }
}

ElementType EnumerationLiteral::getElementType() const {
    return ElementType::ENUMERATION_LITERAL;
}

bool EnumerationLiteral::isSubClassOf(ElementType eType) const {
    bool ret = InstanceSpecification::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::ENUMERATION_LITERAL;
    }

    return ret;
}