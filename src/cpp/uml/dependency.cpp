#include "uml/dependency.h"

using namespace UML;

void Dependency::AddClientFunctor::operator()(NamedElement& el) const {
    if (!m_el->getSources().count(el.getID())) {
        m_el->getSources().add(el);
    }
    m_el->getSupplier().updateCopiedSequenceAddedTo<Dependency>(el, &Dependency::getSupplier);
}

void Dependency::RemoveClientFunctor::operator()(NamedElement& el) const {
    if (m_el->getSources().count(el.getID())) {
        m_el->getSources().remove(el);
    }
    m_el->getSupplier().updateCopiedSequenceRemovedFrom<Dependency>(el, &Dependency::getSupplier);
}

void Dependency::AddSupplierFunctor::operator()(NamedElement& el) const {
    if (!m_el->getTargets().count(el.getID())) {
        m_el->getTargets().add(el);
    }
    m_el->getClient().updateCopiedSequenceAddedTo<Dependency>(el, &Dependency::getClient);
}

void Dependency::RemoveSupplierFunctor::operator()(NamedElement& el) const {
    if (m_el->getTargets().count(el.getID())) {
        m_el->getTargets().remove(el);
    }
    m_el->getClient().updateCopiedSequenceRemovedFrom<Dependency>(el, &Dependency::getClient);
}

void Dependency::setManager(UmlManager* manager) {
    m_client.m_manager = manager;
    m_supplier.m_manager = manager;
    DirectedRelationship::setManager(manager);
    // TODO
}

void Dependency::reindexID(ID oldID, ID newID) {
    DirectedRelationship::reindexID(oldID, newID);
    // TODO
}

Dependency::Dependency() {
    m_client.addProcedures.push_back(new AddClientFunctor(this));
    m_client.removeProcedures.push_back(new RemoveClientFunctor(this));
    m_supplier.addProcedures.push_back(new AddSupplierFunctor(this));
    m_supplier.removeProcedures.push_back(new RemoveSupplierFunctor(this));
}

Dependency::Dependency(const Dependency& dependency) {
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

ElementType Dependency::getElementType() const {
    return ElementType::DEPENDENCY;
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