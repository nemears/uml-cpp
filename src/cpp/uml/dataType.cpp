#include "uml/dataType.h"
// #include "uml/operation.h"
#include "uml/property.h"
#include "uml/uml-stable.h"

using namespace UML;

void DataType::referenceReindexed(ID oldID, ID newID) {
    Classifier::referenceReindexed(oldID, newID);
    m_ownedAttributes.reindex(oldID, newID);
    // m_ownedOperation.reindex(oldID, newID, &DataType::getOwnedOperation);
}

void DataType::referencingReleased(ID id) {
    Classifier::referencingReleased(id);
    m_ownedAttributes.release(id);
    // m_ownedOperation.elementReleased(id, &DataType::getOwnedOperation);
}

void DataType::referenceErased(ID id) {
    Classifier::referenceErased(id);
    m_ownedAttributes.eraseElement(id);
    // m_ownedOperation.elementErased(id);
}

void DataType::init() {
    m_ownedAttributes.subsets(m_attributes);
    m_ownedAttributes.subsets(m_ownedMembers);
    m_ownedAttributes.opposite(&Property::getDataTypeSingleton);
    m_ownedAttributes.m_signature = &DataType::getOwnedAttributes;
}

void DataType::copy(const DataType& rhs) {
    m_ownedAttributes = rhs.m_ownedAttributes;
}

DataType::DataType() : Element(ElementType::DATA_TYPE) {
    init();
}

DataType::~DataType() {
    
}

DataType::DataType(const DataType& rhs) : Element(rhs, ElementType::DATA_TYPE) {
    init();
    NamedElement::copy(rhs);
    Namespace::copy(rhs);
    PackageableElement::copy(rhs);
    Classifier::copy(rhs);
    copy(rhs);
}

Set<Property, DataType>& DataType::getOwnedAttributes() {
    return m_ownedAttributes;
}

// Sequence<Operation>& DataType::getOwnedOperation() {
//     return m_ownedOperation;
// }

bool DataType::isSubClassOf(ElementType eType) const {
    bool ret = Classifier::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::DATA_TYPE;
    }

    return ret;
}