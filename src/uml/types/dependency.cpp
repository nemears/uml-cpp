#include "uml/types/dependency.h"
#include "uml/types/stereotype.h"
#include "uml/types/behavior.h"
#include "uml/types/dataType.h"
#include "uml/types/association.h"
#include "uml/types/interface.h"
#include "uml/types/deployment.h"

using namespace UML;

void Dependency::referenceReindexed(ID newID) {
    PackageableElement::referenceReindexed(newID);
    Relationship::referenceReindexed(newID);
}

// void Dependency::restoreReference(Element* el) {
//     PackageableElement::restoreReference(el);
//     Relationship::restoreReference(el);
//     if (m_suppliers.contains(el->getID())) {
//         el->setReference(this); // need this logic for all setReference top level calls
//     }
// }

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

bool Dependency::isSubClassOf(ElementType eType) const {
    bool ret = DirectedRelationship::isSubClassOf(eType);

    if (!ret) {
        ret = PackageableElement::isSubClassOf(eType);
    }

    if (!ret) {
        ret = eType == ElementType::DEPENDENCY;
    }

    return ret;
}