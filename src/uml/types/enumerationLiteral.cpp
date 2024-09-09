#include "uml/uml-stable.h"

using namespace UML;

Singleton<Enumeration, EnumerationLiteral>& EnumerationLiteral::getEnumerationSingleton() {
    return m_enumeration;
}

EnumerationLiteral::EnumerationLiteral(std::size_t elementType, AbstractManager& manager) : 
    Element(elementType, manager),
    NamedElement(elementType, manager),
    ParameterableElement(elementType, manager),
    InstanceSpecification(elementType, manager)
{
    m_enumeration.subsets(m_namespace);
    m_enumeration.opposite(&Enumeration::getOwnedLiterals);
}

EnumerationPtr EnumerationLiteral::getEnumeration() const {
    return m_enumeration.get();
}

void EnumerationLiteral::setEnumeration(EnumerationPtr enumeration) {
    m_enumeration.set(enumeration);
}

void EnumerationLiteral::setEnumeration(Enumeration& enumeration) {
    m_enumeration.set(enumeration);
}

void EnumerationLiteral::setEnumeration(ID id) {
    m_enumeration.set(id);
}
