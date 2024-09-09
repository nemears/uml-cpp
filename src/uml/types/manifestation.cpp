#include "uml/uml-stable.h"

using namespace UML;

Singleton<PackageableElement, Manifestation>& Manifestation::getUtilizedElementSingleton() {
    return m_utilizedElement;
}

Manifestation::Manifestation(std::size_t elementType, AbstractManager& manager) : 
    Element(elementType, manager),
    NamedElement(elementType, manager),
    ParameterableElement(elementType, manager),
    Abstraction(elementType, manager) 
{
    m_utilizedElement.subsets(m_suppliers);
}

PackageableElementPtr Manifestation::getUtilizedElement() const {
    return m_utilizedElement.get();
}

void Manifestation::setUtilizedElement(PackageableElementPtr utilizedElement) {
    m_utilizedElement.set(utilizedElement);
}

void Manifestation::setUtilizedElement(PackageableElement& utilizedElement) {
    m_utilizedElement.set(utilizedElement);
}

void Manifestation::setUtilizedElement(ID id) {
    m_utilizedElement.set(id);
}
