#include "uml/class.h"
#include "uml/operation.h"
#include "uml/property.h"

using namespace UML;

// void Class::AddOwnedOperationFunctor::operator()(Operation& el) const {
//     if (el.getClass() != m_el) {
//         el.setClass(m_el);
//     }
//     if (!m_el->getFeatures().count(el.getID())) {
//         m_el->getFeatures().add(el);
//     }
//     if (!m_el->getOwnedMembers().count(el.getID())) {
//         m_el->getOwnedMembers().add(el);
//     }
//     updateCopiedSequenceAddedTo(el, &Class::getOwnedOperations);
// }

// void Class::AddOwnedOperationFunctor::operator()(ID id) const {
//     if (!m_el->getOwnedMembers().count(id)) {
//         m_el->getOwnedMembers().addByID(id);
//     }
//     if (!m_el->getFeatures().count(id)) {
//         m_el->getFeatures().addByID(id);
//     }
// }

// void Class::RemoveOwnedOperationFunctor::operator()(Operation& el) const {
//     if (el.getClass() == m_el) {
//         el.setClass(0);
//     }

//     if (m_el->getFeatures().count(el.getID())) {
//         m_el->getFeatures().remove(el);
//     }
//     updateCopiedSequenceRemovedFrom(el, &Class::getOwnedOperations);
// }

// void Class::ClassAddOwnedAttributeFunctor::operator()(Property& el) const {
//     if (el.getClass() != m_el) {
//         el.setClass(m_el);
//     }
// }

// void Class::ClassRemoveOwnedAttributeFunctor::operator()(Property& el) const {
//     if (el.getClass() == m_el) {
//         el.setClass(0);
//     }
// }

// void Class::AddNestedClassifierFunctor::operator()(Classifier& el) const {
//     if (el.getNestingClassID() != m_el->getID()) {
//         el.setNestingClass(m_el);
//     }

//     if (!m_el->getOwnedMembers().count(el.getID())) {
//         m_el->getOwnedMembers().add(el);
//     }

//     if (!m_el->getRedefinedElements().count(el.getID())) {
//         m_el->getRedefinedElements().add(el);
//     }
//     updateCopiedSequenceAddedTo(el, &Class::getNestedClassifiers);
// }

// void Class::AddNestedClassifierFunctor::operator()(ID id) const {
//     if (!m_el->getOwnedMembers().count(id)) {
//         m_el->getOwnedMembers().addByID(id);
//     }
// }

// void Class::RemoveNestedClassifierFunctor::operator()(Classifier& el) const {
//     if (el.getNestingClassID() == m_el->getID()) {
//         el.setNestingClass(0);
//     }

//     if (m_el->getOwnedMembers().count(el.getID())) {
//         m_el->getOwnedMembers().remove(el);
//     }

//     if (m_el->getRedefinedElements().count(el.getID())) {
//         m_el->getRedefinedElements().remove(el);
//     }
//     updateCopiedSequenceRemovedFrom(el, &Class::getNestedClassifiers);
// }

// void Class::referencingReleased(ID id) {
//     StructuredClassifier::referencingReleased(id);
//     BehavioredClassifier::referencingReleased(id);
//     m_ownedOperations.elementReleased(id, &Class::getOwnedOperations);
//     m_nestedClassifiers.elementReleased(id, &Class::getNestedClassifiers);
// }

// void Class::referenceReindexed(ID oldID, ID newID) {
//     StructuredClassifier::referenceReindexed(oldID, newID);
//     BehavioredClassifier::referenceReindexed(oldID, newID);
//     m_ownedOperations.reindex(oldID, newID, &Class::getOwnedOperations);
//     m_nestedClassifiers.reindex(oldID, newID, &Class::getNestedClassifiers);
// }

// void Class::restoreReferences() {
//     StructuredClassifier::restoreReferences();
//     BehavioredClassifier::restoreReferences();
//     m_ownedOperations.restoreReferences();
//     m_nestedClassifiers.restoreReferences();
// }

// void Class::referenceErased(ID id) {
//     StructuredClassifier::referenceErased(id);
//     BehavioredClassifier::referenceErased(id);
//     m_ownedOperations.elementErased(id);
//     m_nestedClassifiers.elementErased(id);
// }

Class::Class() : Element(ElementType::CLASS) {
    // init
}

Class::~Class() {
    
}

Class::Class(const Class& rhs) : Element(rhs, ElementType::CLASS) {
    NamedElement::copy(rhs);
    Namespace::copy(rhs);
    PackageableElement::copy(rhs);
    Classifier::copy(rhs);
    StructuredClassifier::copy(rhs);
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