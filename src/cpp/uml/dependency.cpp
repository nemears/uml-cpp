#include "uml/dependency.h"
#include "uml/uml-stable.h"
#include "uml/setReferenceFunctor.h"

using namespace UML;

void Dependency::referencingReleased(ID id) {
    DirectedRelationship::referencingReleased(id);
    PackageableElement::referencingReleased(id);
}

void Dependency::referenceReindexed(ID oldID, ID newID) {
    PackageableElement::referenceReindexed(oldID, newID);
    Relationship::referenceReindexed(oldID, newID);
}

void Dependency::restoreReference(Element* el) {
    PackageableElement::restoreReference(el);
    if (m_supplier.contains(el->getID())) {
        el->setReference(this); // need this logic for all setReference top level calls
    }
}

void Dependency::referenceErased(ID id) {
    PackageableElement::referenceErased(id);
    DirectedRelationship::referenceErased(id);
}

void Dependency::init() {
    m_client.subsets(m_sources);
    m_client.opposite(&NamedElement::getClientDependencies);
    m_client.m_signature = &Dependency::getClient;
    m_supplier.subsets(m_targets);
    m_supplier.m_signature = &Dependency::getSupplier;
    m_supplier.m_addFunctors.insert(new SetReferenceFunctor(this));
    m_supplier.m_removeFunctors.insert(new RemoveReferenceFunctor(this));
}

void Dependency::copy(const Dependency& rhs) {
    m_client = Set<NamedElement, Dependency>(rhs.m_client);
    m_client.m_el = this;
    m_supplier = Set<NamedElement, Dependency>(rhs.m_supplier);
    m_supplier.m_el = this;
}

Dependency::Dependency() : Element(ElementType::DEPENDENCY) {
    init();
}

Dependency::Dependency(const Dependency& rhs) : Element(rhs, ElementType::DEPENDENCY) {
    init();
    Relationship::copy(rhs);
    DirectedRelationship::copy(rhs);
    NamedElement::copy(rhs);
    ParameterableElement::copy(rhs);
    PackageableElement::copy(rhs);
    copy(rhs);
}

Dependency::~Dependency() {

}

Set<NamedElement, Dependency>& Dependency::getClient() {
    return m_client;
}

Set<NamedElement, Dependency>& Dependency::getSupplier() {
    return m_supplier;
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