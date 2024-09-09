#include "uml/uml-stable.h"

using namespace UML;

Class::Class(std::size_t elementType, AbstractManager& manager) : 
    Element(elementType, manager),
    NamedElement(elementType, manager),
    ParameterableElement(elementType, manager),
    PackageableElement(elementType, manager),
    Classifier(elementType, manager),
    EncapsulatedClassifier(elementType, manager),
    BehavioredClassifier(elementType, manager)
{
    m_classOwnedAttrubutes.redefines(m_ownedAttributes);
    m_classOwnedAttrubutes.opposite(&Property::getClassSingleton);
    m_ownedOperations.subsets(m_features);
    m_ownedOperations.subsets(m_ownedMembers);
    m_ownedOperations.opposite(&Operation::getClassSingleton);
    m_nestedClassifiers.subsets(m_ownedMembers);
    m_ownedReceptions.subsets(m_features);
    m_ownedReceptions.subsets(m_ownedMembers);
}

IndexableOrderedSet<Property, Class>& Class::getOwnedAttributes() {
    return m_classOwnedAttrubutes;
}

IndexableOrderedSet<Operation, Class>& Class::getOwnedOperations() {
    return m_ownedOperations;
}

IndexableOrderedSet<Classifier, Class>& Class::getNestedClassifiers() {
    return m_nestedClassifiers;
}

IndexableSet<Reception, Class>& Class::getOwnedReceptions() {
    return m_ownedReceptions;
}
