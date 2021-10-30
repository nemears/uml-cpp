#include "uml/dependency.h"

using namespace UML;

void Dependency::AddClientFunctor::operator()(NamedElement& el) const {
    if (!m_el->getSources().count(el.getID())) {
        m_el->getSources().add(el);
    }
    if (!el.getClientDependencies().count(m_el->getID())) {
        el.getClientDependencies().add(*m_el);
    }
    updateCopiedSequenceAddedTo(el, &Dependency::getClient);
}

void Dependency::AddClientFunctor::operator()(ID id) const {
    if (!m_el->getSources().count(id)) {
        m_el->getSources().addByID(id);
    }
}

void Dependency::RemoveClientFunctor::operator()(NamedElement& el) const {
    if (m_el->getSources().count(el.getID())) {
        m_el->getSources().remove(el);
    }
    if (el.getClientDependencies().count(m_el->getID())) {
        el.getClientDependencies().remove(*m_el);
    }
    updateCopiedSequenceRemovedFrom(el, &Dependency::getClient);
}

void Dependency::AddSupplierFunctor::operator()(NamedElement& el) const {
    if (!m_el->getTargets().count(el.getID())) {
        m_el->getTargets().add(el);
    }
    if (!el.getSupplierDependencies().count(m_el->getID())) {
        el.getSupplierDependencies().add(*m_el);
    }
    updateCopiedSequenceAddedTo(el, &Dependency::getSupplier);
}

void Dependency::AddSupplierFunctor::operator()(ID id) const {
    if (!m_el->getTargets().count(id)) {
        m_el->getTargets().addByID(id);
    }
}

void Dependency::RemoveSupplierFunctor::operator()(NamedElement& el) const {
    if (m_el->getTargets().count(el.getID())) {
        m_el->getTargets().remove(el);
    }
    if (el.getSupplierDependencies().count(m_el->getID())) {
        el.getSupplierDependencies().remove(*m_el);
    }
    updateCopiedSequenceRemovedFrom(el, &Dependency::getSupplier);
}

void Dependency::referencingReleased(ID id) {
    DirectedRelationship::referencingReleased(id);
    PackageableElement::referencingReleased(id);
    m_client.elementReleased(id, &Dependency::getClient);
    m_supplier.elementReleased(id, &Dependency::getSupplier);
}

void Dependency::referenceReindexed(ID oldID, ID newID) {
    PackageableElement::referenceReindexed(oldID, newID);
    Relationship::referenceReindexed(oldID, newID);
    m_client.reindex(oldID, newID, &Dependency::getClient);
    m_supplier.reindex(oldID, newID, &Dependency::getSupplier);
}

void Dependency::restoreReferences() {
    PackageableElement::restoreReferences();
    DirectedRelationship::restoreReferences();
    m_client.restoreReferences();
    m_supplier.restoreReferences();
}

void Dependency::referenceErased(ID id) {
    PackageableElement::referenceErased(id);
    DirectedRelationship::referenceErased(id);
    m_client.elementErased(id);
    m_supplier.elementErased(id);
}

Dependency::Dependency() : Element(ElementType::DEPENDENCY) {
    m_client.addProcedures.push_back(new AddClientFunctor(this));
    m_client.removeProcedures.push_back(new RemoveClientFunctor(this));
    m_supplier.addProcedures.push_back(new AddSupplierFunctor(this));
    m_supplier.removeProcedures.push_back(new RemoveSupplierFunctor(this));
}

Dependency::Dependency(const Dependency& dependency) : Element(dependency, ElementType::DEPENDENCY) {
    m_client.m_el = this;
    m_supplier.m_el = this;
    m_client.addProcedures.clear();
    m_supplier.addProcedures.clear();
    m_client.addProcedures.push_back(new AddClientFunctor(this));
    m_client.removeProcedures.push_back(new RemoveClientFunctor(this));
    m_supplier.addProcedures.push_back(new AddSupplierFunctor(this));
    m_supplier.removeProcedures.push_back(new RemoveSupplierFunctor(this));
}

Dependency::~Dependency() {

}

Sequence<NamedElement>& Dependency::getClient() {
    return m_client;
}

Sequence<NamedElement>& Dependency::getSupplier() {
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