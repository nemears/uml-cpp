#include "uml/dependency.h"
#include "uml/package.h"
#include "uml/packageMerge.h"

using namespace UML;

void Dependency::referencingReleased(ID id) {
    DirectedRelationship::referencingReleased(id);
    PackageableElement::referencingReleased(id);
    m_client.release(id);
    m_supplier.release(id);
}

void Dependency::referenceReindexed(ID oldID, ID newID) {
    PackageableElement::referenceReindexed(oldID, newID);
    Relationship::referenceReindexed(oldID, newID);
    m_client.reindex(oldID, newID);
    m_supplier.reindex(oldID, newID);
}

void Dependency::restoreReferences() {
    PackageableElement::restoreReferences();
    DirectedRelationship::restoreReferences();
    // m_client.restoreReferences();
    // m_supplier.restoreReferences();
}

void Dependency::referenceErased(ID id) {
    // PackageableElement::referenceErased(id);
    // DirectedRelationship::referenceErased(id);
    m_client.eraseElement(id);
    m_supplier.eraseElement(id);
}

void Dependency::init() {
    m_client.subsets(m_sources);
    m_client.opposite(&NamedElement::getClientDependencies);
    m_client.m_signature = &Dependency::getClient;
    m_supplier.subsets(m_targets);
    m_supplier.m_signature = &Dependency::getSupplier;
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
    Relationship::copy(rhs);
    DirectedRelationship::copy(rhs);
    NamedElement::copy(rhs);
    PackageableElement::copy(rhs);
    copy(rhs);
    Relationship::init();
    DirectedRelationship::init();
    NamedElement::init();
    PackageableElement::init();
    init();
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