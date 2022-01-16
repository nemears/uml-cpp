#include "uml/dataType.h"
#include "uml/operation.h"
#include "uml/property.h"
#include "uml/uml-stable.h"

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
    m_ownedAttributes.m_signature = &DataType::getOwnedAttributesSet;
    m_ownedOperations.subsets(m_features);
    m_ownedOperations.subsets(m_ownedMembers);
    m_ownedOperations.opposite(&Operation::getDataTypeSingleton);
    m_ownedOperations.m_signature = &DataType::getOwnedOperationsSet;
}

void DataType::copy(const DataType& rhs) {
    m_ownedAttributes = rhs.m_ownedAttributes;
    m_ownedOperations = rhs.m_ownedOperations;
}

DataType::DataType() : Element(ElementType::DATA_TYPE) {
    init();
}

DataType::~DataType() {
    
}

DataType::DataType(const DataType& rhs) : Element(rhs, ElementType::DATA_TYPE) {
    init();
    Element::copy(rhs);
    NamedElement::copy(rhs);
    Namespace::copy(rhs);
    ParameterableElement::copy(rhs);
    PackageableElement::copy(rhs);
    TemplateableElement::copy(rhs);
    RedefinableElement::copy(rhs);
    Classifier::copy(rhs);
    copy(rhs);
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