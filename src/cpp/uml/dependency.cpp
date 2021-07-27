#include "uml/dependency.h"

using namespace UML;

void Dependency::AddClientFunctor::operator()(Element& el) const {
    if (!m_el->as<Dependency>().getSources().count(el.getID())) {
        m_el->as<Dependency>().getSources().add(el.as<NamedElement>());
    }
}

void Dependency::RemoveClientFunctor::operator()(Element& el) const {
    if (m_el->as<Dependency>().getSources().count(el.getID())) {
        m_el->as<Dependency>().getSources().remove(el.as<NamedElement>());
    }
}

void Dependency::AddSupplierFunctor::operator()(Element& el) const {
    if (!m_el->as<Dependency>().getTargets().count(el.getID())) {
        m_el->as<Dependency>().getTargets().add(el.as<NamedElement>());
    }
}

void Dependency::RemoveSupplierFunctor::operator()(Element& el) const {
    if (m_el->as<Dependency>().getTargets().count(el.getID())) {
        m_el->as<Dependency>().getTargets().remove(el.as<NamedElement>());
    }
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