#include "uml/enumerationLiteral.h"
#include "uml/enumeration.h"

using namespace UML;

EnumerationLiteral::EnumerationLiteral() {
    m_enumeration = 0;
}

Enumeration* EnumerationLiteral::getEnumeration() {
    return m_enumeration;
}

void EnumerationLiteral::setEnumeration(Enumeration* enumeration) {
    if (m_enumeration) {
        if (m_enumeration != enumeration) {
            if (m_enumeration->getOwnedLiteral().count(m_id)) {
                m_enumeration->getOwnedLiteral().remove(*this);
            }
        }
    }
    m_enumeration = enumeration;
    if (m_enumeration) {
        if (!m_enumeration->getOwnedLiteral().count(m_id)) {
            m_enumeration->getOwnedLiteral().add(*this);
        }
    }
}

ElementType EnumerationLiteral::getElementType() const {
    return ElementType::ENUMERATION_LITERAL;
}

bool EnumerationLiteral::isSubClassOf(ElementType eType) {
    bool ret = InstanceSpecification::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::ENUMERATION_LITERAL;
    }

    return ret;
}