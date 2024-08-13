#include "uml/uml-stable.h"

using namespace UML;

DataType::DataType() : Element(ElementType::DATA_TYPE) {
    m_ownedAttributes.subsets(m_attributes);
    m_ownedAttributes.subsets(m_ownedMembers);
    m_ownedAttributes.opposite(&Property::getDataTypeSingleton);
    m_ownedOperations.subsets(m_features);
    m_ownedOperations.subsets(m_ownedMembers);
    m_ownedOperations.opposite(&Operation::getDataTypeSingleton);
}

DataType::~DataType() {
    
}

IndexableOrderedSet<Property, DataType>& DataType::getOwnedAttributes() {
    return m_ownedAttributes;
}

IndexableOrderedSet<Operation, DataType>& DataType::getOwnedOperations() {
    return m_ownedOperations;
}

bool DataType::is(ElementType eType) const {
    bool ret = Classifier::is(eType);

    if (!ret) {
        ret = eType == ElementType::DATA_TYPE;
    }

    return ret;
}
