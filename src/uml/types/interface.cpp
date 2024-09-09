#include "uml/uml-stable.h"

using namespace UML;

Interface::Interface(std::size_t elementType, AbstractManager& manager) : 
    Element(elementType, manager),
    NamedElement(elementType, manager),
    ParameterableElement(elementType, manager),
    PackageableElement(elementType, manager),
    Classifier(elementType, manager)
{
    m_ownedAttributes.subsets(m_ownedMembers);
    m_ownedAttributes.subsets(m_attributes);
    m_ownedAttributes.opposite(&Property::getInterfaceSingleton);
    m_ownedOperations.subsets(m_features);
    m_ownedOperations.subsets(m_ownedMembers);
    m_ownedOperations.opposite(&Operation::getInterfaceSingleton);
    m_nestedClassifiers.subsets(m_ownedMembers);
}

OrderedSet<Property, Interface>& Interface::getOwnedAttributes() {
    return m_ownedAttributes;
}

OrderedSet<Operation, Interface>& Interface::getOwnedOperations() {
    return m_ownedOperations;
}

OrderedSet<Classifier, Interface>& Interface::getNestedClassifiers() {
    return m_nestedClassifiers;
}
