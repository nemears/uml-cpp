#include "uml/uml-stable.h"

using namespace UML;

DataType::DataType(std::size_t elementType, AbstractManager& manager) : 
    Element(elementType, manager),
    NamedElement(elementType, manager),
    ParameterableElement(elementType, manager),
    PackageableElement(elementType, manager),
    Classifier(elementType, manager)
{
    m_ownedAttributes.subsets(m_attributes);
    m_ownedAttributes.subsets(m_ownedMembers);
    m_ownedAttributes.opposite(&Property::getDataTypeSingleton);
    m_ownedOperations.subsets(m_features);
    m_ownedOperations.subsets(m_ownedMembers);
    m_ownedOperations.opposite(&Operation::getDataTypeSingleton);
}

IndexableOrderedSet<Property, DataType>& DataType::getOwnedAttributes() {
    return m_ownedAttributes;
}

IndexableOrderedSet<Operation, DataType>& DataType::getOwnedOperations() {
    return m_ownedOperations;
}
