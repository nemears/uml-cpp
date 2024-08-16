#include "uml/uml-stable.h"

using namespace UML;

void Dependency::referenceErased(ID id) {
    PackageableElement::referenceErased(id);
    DirectedRelationship::referenceErased(id);
}

Dependency::Dependency() : Element(ElementType::DEPENDENCY) {
    m_clients.subsets(m_sources);
    m_clients.opposite(&NamedElement::getClientDependencies);
    m_suppliers.subsets(m_targets);
}

Dependency::~Dependency() {
    
}

Set<NamedElement, Dependency>& Dependency::getClients() {
    return m_clients;
}

Set<NamedElement, Dependency>& Dependency::getSuppliers() {
    return m_suppliers;
}

bool Dependency::is(ElementType eType) const {
    bool ret = DirectedRelationship::is(eType);

    if (!ret) {
        ret = PackageableElement::is(eType);
    }

    if (!ret) {
        ret = eType == ElementType::DEPENDENCY;
    }

    return ret;
}
