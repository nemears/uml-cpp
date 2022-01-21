#include "uml/interface.h"
#include "uml/uml-stable.h"

using namespace UML;

Set<Property, Interface>& Interface::getOwnedAttributesSet() {
    return m_ownedAttributes;
}

Set<Operation, Interface>& Interface::getOwnedOperationsSet() {
    return m_ownedOperations;
}

Set<Classifier, Interface>& Interface::getNestedClassifiersSet() {
    return m_nestedClassifiers;
}

void Interface::init() {
    m_ownedAttributes.subsets(m_ownedMembers);
    m_ownedAttributes.subsets(m_attributes);
    m_ownedAttributes.opposite(&Property::getInterfaceSingleton);
    m_ownedAttributes.m_signature = &Interface::getOwnedAttributesSet;
    m_ownedOperations.subsets(m_features);
    m_ownedOperations.subsets(m_ownedMembers);
    m_ownedOperations.opposite(&Operation::getInterfaceSingleton);
    m_ownedOperations.m_signature = &Interface::getOwnedOperationsSet;
    m_nestedClassifiers.subsets(m_ownedMembers);
    m_nestedClassifiers.m_signature = &Interface::getNestedClassifiersSet;
}

void Interface::copy(const Interface& rhs) {
    m_ownedAttributes = rhs.m_ownedAttributes;
    m_ownedOperations = rhs.m_ownedOperations;
    m_nestedClassifiers = rhs.m_nestedClassifiers;
}

Interface::Interface() : Element(ElementType::INTERFACE) {
    init();
}

Interface::~Interface() {

}

Interface::Interface(const Interface& rhs) : Element(rhs, ElementType::INTERFACE) {
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

OrderedSet<Property, Interface>& Interface::getOwnedAttributes() {
    return m_ownedAttributes;
}

OrderedSet<Operation, Interface>& Interface::getOwnedOperations() {
    return m_ownedOperations;
}

OrderedSet<Classifier, Interface>& Interface::getNestedClassifiers() {
    return m_nestedClassifiers;
}

bool Interface::isSubClassOf(ElementType eType) const {
    bool ret = Classifier::isSubClassOf(eType);
    
    if (!ret) {
        ret = eType == ElementType::INTERFACE;
    }

    return ret;
}