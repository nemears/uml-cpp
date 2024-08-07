#include "uml/uml-stable.h"

using namespace UML;

Singleton<Enumeration, EnumerationLiteral>& EnumerationLiteral::getEnumerationSingleton() {
    return m_enumeration;
}

EnumerationLiteral::EnumerationLiteral() : Element(ElementType::ENUMERATION_LITERAL) {
    m_enumeration.subsets(*m_namespace);
    m_enumeration.opposite(&Enumeration::getOwnedLiterals);
}

EnumerationLiteral::~EnumerationLiteral() {
    
}

EnumerationPtr EnumerationLiteral::getEnumeration() const {
    return m_enumeration.get();
}

void EnumerationLiteral::setEnumeration(Enumeration* enumeration) {
    m_enumeration.set(enumeration);
}

void EnumerationLiteral::setEnumeration(Enumeration& enumeration) {
    m_enumeration.set(enumeration);
}

void EnumerationLiteral::setEnumeration(ID id) {
    m_enumeration.set(id);
}

bool EnumerationLiteral::is(ElementType eType) const {
    bool ret = InstanceSpecification::is(eType);

    if (!ret) {
        ret = eType == ElementType::ENUMERATION_LITERAL;
    }

    return ret;
}
