#include "uml/dependency.h"
#include "uml/setReferenceFunctor.h"
#include "uml/stereotype.h"
#include "uml/behavior.h"
#include "uml/dataType.h"
#include "uml/association.h"
#include "uml/interface.h"
#include "uml/deployment.h"

using namespace UML;

void Dependency::referenceReindexed(ID oldID, ID newID) {
    PackageableElement::referenceReindexed(oldID, newID);
    Relationship::referenceReindexed(oldID, newID);
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

void Dependency::init() {
    m_clients.subsets(m_sources);
    m_clients.opposite(&NamedElement::getClientDependencies);
    m_suppliers.subsets(m_targets);
}

Dependency::Dependency() : Element(ElementType::DEPENDENCY) {
    init();
}

Dependency::~Dependency() {
    mountAndRelease();
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