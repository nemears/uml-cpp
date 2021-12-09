#include "uml/dataType.h"
// #include "uml/operation.h"
#include "uml/property.h"
#include "uml/uml-stable.h"

using namespace UML;

void DataType::referenceReindexed(ID oldID, ID newID) {
    Classifier::referenceReindexed(oldID, newID);
    m_ownedAttributes.reindex(oldID, newID);
    m_ownedOperations.reindex(oldID, newID);
}

void DataType::referencingReleased(ID id) {
    Classifier::referencingReleased(id);
    m_ownedAttributes.release(id);
    m_ownedOperations.release(id);
}

void DataType::referenceErased(ID id) {
    Classifier::referenceErased(id);
    m_ownedAttributes.eraseElement(id);
    m_ownedOperations.eraseElement(id);
}

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

    m_redefinedElement.subsets(m_ownedOperations);
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