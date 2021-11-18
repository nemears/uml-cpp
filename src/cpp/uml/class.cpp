#include "uml/class.h"
// #include "uml/operation.h"
#include "uml/property.h"
#include "uml/uml-stable.h"

using namespace UML;

void Class::referencingReleased(ID id) {
    StructuredClassifier::referencingReleased(id);
    m_classOwnedAttrubutes.release(id);
    // BehavioredClassifier::referencingReleased(id);
    // m_ownedOperations.elementReleased(id, &Class::getOwnedOperations);
    // m_nestedClassifiers.elementReleased(id, &Class::getNestedClassifiers);
}

void Class::referenceReindexed(ID oldID, ID newID) {
    StructuredClassifier::referenceReindexed(oldID, newID);
    m_classOwnedAttrubutes.reindex(oldID, newID);
    // BehavioredClassifier::referenceReindexed(oldID, newID);
    // m_ownedOperations.reindex(oldID, newID, &Class::getOwnedOperations);
    // m_nestedClassifiers.reindex(oldID, newID, &Class::getNestedClassifiers);
}

void Class::restoreReferences() {
    StructuredClassifier::restoreReferences();
    // BehavioredClassifier::restoreReferences();
    // m_ownedOperations.restoreReferences();
    // m_nestedClassifiers.restoreReferences();
}

void Class::referenceErased(ID id) {
    StructuredClassifier::referenceErased(id);
    m_classOwnedAttrubutes.eraseElement(id);
    // BehavioredClassifier::referenceErased(id);
    // m_ownedOperations.elementErased(id);
    // m_nestedClassifiers.elementErased(id);
}

void Class::init() {
    m_classOwnedAttrubutes.redefines(m_ownedAttributes);
    m_classOwnedAttrubutes.opposite(&Property::getClassSingleton);
    m_classOwnedAttrubutes.m_signature = &Class::getOwnedAttributes;
}

void Class::copy(const Class& rhs) {
    m_classOwnedAttrubutes = rhs.m_classOwnedAttrubutes;
}

Class::Class() : Element(ElementType::CLASS) {
    init();
}

Class::~Class() {
    
}

Class::Class(const Class& rhs) : Element(rhs, ElementType::CLASS) {
    init();
    NamedElement::copy(rhs);
    Namespace::copy(rhs);
    PackageableElement::copy(rhs);
    Classifier::copy(rhs);
    StructuredClassifier::copy(rhs);
}

Set<Property, Class>& Class::getOwnedAttributes() {
    return m_classOwnedAttrubutes;
}

// Sequence<Operation>& Class::getOwnedOperations() {
//     return m_ownedOperations;
// }
// Sequence<Classifier>& Class::getNestedClassifiers() {
//     return m_nestedClassifiers;
// }

bool Class::isSubClassOf(ElementType eType) const {
    bool ret = StructuredClassifier::isSubClassOf(eType);

    // if (!ret) {
    //     ret = BehavioredClassifier::isSubClassOf(eType);
    // }

    if (!ret) {
        ret = eType == ElementType::CLASS;
    }
    
    return ret;
}