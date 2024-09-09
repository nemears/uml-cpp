#include "uml/uml-stable.h"

using namespace UML;

Dependency::Dependency(std::size_t elementType, AbstractManager& manager) : 
    Element(elementType, manager) ,
    NamedElement(elementType, manager),
    ParameterableElement(elementType, manager),
    DirectedRelationship(elementType, manager),
    PackageableElement(elementType, manager)
{
    m_clients.subsets(m_sources);
    m_clients.opposite(&NamedElement::getClientDependencies);
    m_suppliers.subsets(m_targets);
}

Set<NamedElement, Dependency>& Dependency::getClients() {
    return m_clients;
}

Set<NamedElement, Dependency>& Dependency::getSuppliers() {
    return m_suppliers;
}
