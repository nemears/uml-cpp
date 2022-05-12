#include "uml/dataType.h"
#include "uml/operation.h"
#include "uml/property.h"
#include "uml/generalization.h"
#include "uml/package.h"
#include "uml/behavior.h"
#include "uml/dataType.h"
#include "uml/association.h"
#include "uml/stereotype.h"
#include "uml/interface.h"
#include "uml/deployment.h"

using namespace UML;

Set<Property, DataType>& DataType::getOwnedAttributesSet() {
    return m_ownedAttributes;
}

Set<Operation, DataType>& DataType::getOwnedOperationsSet() {
    return m_ownedOperations;
}

void DataType::init() {
    m_ownedAttributes.subsets(m_attributes);
    m_ownedAttributes.subsets(m_ownedMembers);
    m_ownedAttributes.opposite(&Property::getDataTypeSingleton);
    m_ownedOperations.subsets(m_features);
    m_ownedOperations.subsets(m_ownedMembers);
    m_ownedOperations.opposite(&Operation::getDataTypeSingleton);
}

DataType::DataType() : Element(ElementType::DATA_TYPE) {
    init();
}

DataType::~DataType() {
    mountAndRelease();
}

OrderedSet<Property, DataType>& DataType::getOwnedAttributes() {
    return m_ownedAttributes;
}

OrderedSet<Operation, DataType>& DataType::getOwnedOperations() {
    return m_ownedOperations;
}

bool DataType::isSubClassOf(ElementType eType) const {
    bool ret = Classifier::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::DATA_TYPE;
    }

    return ret;
}