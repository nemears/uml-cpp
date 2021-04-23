#include "uml/dataType.h"

using namespace UML;

DataType::DataType() {
    m_ownedAttribute = new Sequence<Property>;
    m_ownedOperation = new Sequence<Operation>;
}

DataType::~DataType() {
    delete m_ownedAttribute;
    delete m_ownedOperation;
}

DataType::DataType(const DataType& el) {
    m_ownedAttribute = new Sequence<Property>(*el.m_ownedAttribute);
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