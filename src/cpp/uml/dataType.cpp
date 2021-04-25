#include "uml/dataType.h"

using namespace UML;

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

DataType::DataType() {
    m_ownedAttribute = new Sequence<Property>;
    m_ownedAttribute->addProcedures.push_back(new AddOwnedAttributeFunctor(this));
    m_ownedAttribute->removeProcedures.push_back(new RemoveOwnedAttributeFunctor(this));
    m_ownedOperation = new Sequence<Operation>;
}

DataType::~DataType() {
    delete m_ownedAttribute;
    delete m_ownedOperation;
}

DataType::DataType(const DataType& el) {
    m_ownedAttribute = new Sequence<Property>(*el.m_ownedAttribute);
    m_ownedAttribute->addProcedures.clear();
    m_ownedAttribute->addProcedures.push_back(new AddOwnedAttributeFunctor(this));
    m_ownedAttribute->removeProcedures.clear();
    m_ownedAttribute->removeProcedures.push_back(new RemoveOwnedAttributeFunctor(this));
    m_ownedOperation = new Sequence<Operation>(*el.m_ownedOperation);
}

Sequence<Property>& DataType::getOwnedAttribute() {
    return *m_ownedAttribute;
}

Sequence<Operation>& DataType::getOwnedOperation() {
    return *m_ownedOperation;
}

ElementType DataType::getElementType() {
    return ElementType::DATA_TYPE;
}

bool DataType::isSubClassOf(ElementType eType) {
    bool ret = Classifier::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::DATA_TYPE;
    }

    return ret;
}