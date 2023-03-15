#include "uml/types/dataType.h"
#include "uml/types/operation.h"
#include "uml/types/property.h"
#include "uml/types/generalization.h"
#include "uml/types/package.h"
#include "uml/types/behavior.h"
#include "uml/types/association.h"
#include "uml/types/stereotype.h"
#include "uml/types/interface.h"
#include "uml/types/deployment.h"

using namespace UML;

TypedSet<Property, DataType>& DataType::getOwnedAttributesSet() {
    return m_ownedAttributes;
}

TypedSet<Operation, DataType>& DataType::getOwnedOperationsSet() {
    return m_ownedOperations;
}

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