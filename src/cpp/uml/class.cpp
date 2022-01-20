#include "uml/class.h"
#include "uml/operation.h"
#include "uml/property.h"
#include "uml/uml-stable.h"

using namespace UML;

Set<Property, Class>& Class::getOwnedAttributesSet() {
    return m_classOwnedAttrubutes;
}

Set<Operation, Class>& Class::getOwnedOperationsSet() {
    return m_ownedOperations;
}

Set<Classifier, Class>& Class::getNestedClassifiersSet() {
    return m_nestedClassifiers;
}

void Class::init() {
    m_classOwnedAttrubutes.redefines(m_ownedAttributes);
    m_classOwnedAttrubutes.opposite(&Property::getClassSingleton);
    m_classOwnedAttrubutes.m_signature = &Class::getOwnedAttributesSet;
    m_ownedOperations.subsets(m_features);
    m_ownedOperations.subsets(m_ownedMembers);
    m_ownedOperations.opposite(&Operation::getClassSingleton);
    m_ownedOperations.m_signature = &Class::getOwnedOperationsSet;
    m_nestedClassifiers.subsets(m_ownedMembers);
    m_nestedClassifiers.m_signature = &Class::getNestedClassifiersSet;
}

void Class::copy(const Class& rhs) {
    m_classOwnedAttrubutes = rhs.m_classOwnedAttrubutes;
    m_ownedOperations = rhs.m_ownedOperations;
    m_nestedClassifiers = rhs.m_nestedClassifiers;
}

Class::Class() : Element(ElementType::CLASS) {
    init();
}

Class::~Class() {
    
}

Class::Class(const Class& rhs) : Element(rhs, ElementType::CLASS) {
    init();
    Element::copy(rhs);
    NamedElement::copy(rhs);
    Namespace::copy(rhs);
    ParameterableElement::copy(rhs);
    PackageableElement::copy(rhs);
    TemplateableElement::copy(rhs);
    RedefinableElement::copy(rhs);
    Classifier::copy(rhs);
    StructuredClassifier::copy(rhs);
    EncapsulatedClassifier::copy(rhs);
    BehavioredClassifier::copy(rhs);
    copy(rhs);
}

OrderedSet<Property, Class>& Class::getOwnedAttributes() {
    return m_classOwnedAttrubutes;
}

OrderedSet<Operation, Class>& Class::getOwnedOperations() {
    return m_ownedOperations;
}

OrderedSet<Classifier, Class>& Class::getNestedClassifiers() {
    return m_nestedClassifiers;
}

bool Class::isSubClassOf(ElementType eType) const {
    bool ret = EncapsulatedClassifier::isSubClassOf(eType);

    if (!ret) {
        ret = BehavioredClassifier::isSubClassOf(eType);
    }

    if (!ret) {
        ret = eType == ElementType::CLASS;
    }
    
    return ret;
}