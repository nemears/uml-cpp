#include "uml/dataType.h"
#include "uml/operation.h"

using namespace UML;

void DataType::setManager(UmlManager* manager) {
    Classifier::setManager(manager);
    m_ownedAttribute.m_manager = manager;
    m_ownedOperation.m_manager = manager;
}

void DataType::AddOwnedAttributeFunctor::operator()(Element& el) const {
    if (!dynamic_cast<DataType*>(m_el)->getAttributes().count(el.getID())) {
        dynamic_cast<DataType*>(m_el)->getAttributes().add(dynamic_cast<Property&>(el));
    }

    if (!dynamic_cast<DataType*>(m_el)->getOwnedMembers().count(el.getID())) {
        dynamic_cast<DataType*>(m_el)->getOwnedMembers().add(dynamic_cast<Property&>(el));
    }

    if (dynamic_cast<Property&>(el).getDataType() != m_el) {
        dynamic_cast<Property&>(el).setDataType(dynamic_cast<DataType*>(m_el));
    }
}

void DataType::RemoveOwnedAttributeFunctor::operator()(Element& el) const {
    if (dynamic_cast<DataType*>(m_el)->getAttributes().count(el.getID())) {
        dynamic_cast<DataType*>(m_el)->getAttributes().remove(dynamic_cast<Property&>(el));
    }

    if (dynamic_cast<DataType*>(m_el)->getOwnedMembers().count(el.getID())) {
        dynamic_cast<DataType*>(m_el)->getOwnedMembers().remove(dynamic_cast<Property&>(el));
    }

    if (dynamic_cast<Property&>(el).getDataType() == m_el) {
        dynamic_cast<Property&>(el).setDataType(0);
    }
}

void DataType::AddOwnedOperationFunctor::operator()(Element& el) const {
    if (!dynamic_cast<DataType*>(m_el)->getFeatures().count(el.getID())) {
        dynamic_cast<DataType*>(m_el)->getFeatures().add(dynamic_cast<Operation&>(el));
    }

    if (!dynamic_cast<DataType*>(m_el)->getOwnedMembers().count(el.getID())) {
        dynamic_cast<DataType*>(m_el)->getOwnedMembers().add(dynamic_cast<Operation&>(el));
    }

    if (dynamic_cast<Operation&>(el).getDataType() != m_el) {
        dynamic_cast<Operation&>(el).setDataType(dynamic_cast<DataType*>(m_el));
    }

    if (!dynamic_cast<Operation&>(el).getRedefinitionContext().count(m_el->getID())) {
        dynamic_cast<Operation&>(el).getRedefinitionContext().add(*dynamic_cast<DataType*>(m_el));
    }
}

void DataType::RemoveOwnedOperationFunctor::operator()(Element& el) const {
    if (dynamic_cast<DataType*>(m_el)->getFeatures().count(el.getID())) {
        dynamic_cast<DataType*>(m_el)->getFeatures().remove(dynamic_cast<Operation&>(el));
    }

    if (dynamic_cast<DataType*>(m_el)->getOwnedMembers().count(el.getID())) {
        dynamic_cast<DataType*>(m_el)->getOwnedMembers().remove(dynamic_cast<Operation&>(el));
    }

    if (dynamic_cast<Operation&>(el).getDataType() == m_el) {
        dynamic_cast<Operation&>(el).setDataType(0);
    }

    if (dynamic_cast<Operation&>(el).getRedefinitionContext().count(m_el->getID())) {
        dynamic_cast<Operation&>(el).getRedefinitionContext().remove(*dynamic_cast<DataType*>(m_el));
    }
}

DataType::DataType() {
    m_ownedAttribute.addProcedures.push_back(new AddOwnedAttributeFunctor(this));
    m_ownedAttribute.removeProcedures.push_back(new RemoveOwnedAttributeFunctor(this));
    m_ownedOperation.addProcedures.push_back(new AddOwnedOperationFunctor(this));
    m_ownedOperation.removeProcedures.push_back(new RemoveOwnedOperationFunctor(this));
}

DataType::~DataType() {
    
}

DataType::DataType(const DataType& el) : Classifier(el) , PackageableElement(el), NamedElement(el), Element(el){
    m_ownedAttribute = el.m_ownedAttribute;
    m_ownedAttribute.addProcedures.clear();
    m_ownedAttribute.addProcedures.push_back(new AddOwnedAttributeFunctor(this));
    m_ownedAttribute.removeProcedures.clear();
    m_ownedAttribute.removeProcedures.push_back(new RemoveOwnedAttributeFunctor(this));
    m_ownedOperation = el.m_ownedOperation;
    m_ownedOperation.addProcedures.clear();
    m_ownedOperation.addProcedures.push_back(new AddOwnedOperationFunctor(this));
    m_ownedOperation.removeProcedures.clear();
    m_ownedOperation.removeProcedures.push_back(new RemoveOwnedOperationFunctor(this));
}

Sequence<Property>& DataType::getOwnedAttribute() {
    return m_ownedAttribute;
}

Sequence<Operation>& DataType::getOwnedOperation() {
    return m_ownedOperation;
}

ElementType DataType::getElementType() const {
    return ElementType::DATA_TYPE;
}

bool DataType::isSubClassOf(ElementType eType) const {
    bool ret = Classifier::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::DATA_TYPE;
    }

    return ret;
}