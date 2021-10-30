#include "uml/class.h"
#include "uml/operation.h"
#include "uml/property.h"

using namespace UML;

void Class::AddOwnedOperationFunctor::operator()(Operation& el) const {
    if (el.getClass() != m_el) {
        el.setClass(m_el);
    }
    if (!m_el->getFeatures().count(el.getID())) {
        m_el->getFeatures().add(el);
    }
    if (!m_el->getOwnedMembers().count(el.getID())) {
        m_el->getOwnedMembers().add(el);
    }
    updateCopiedSequenceAddedTo(el, &Class::getOwnedOperations);
}

void Class::AddOwnedOperationFunctor::operator()(ID id) const {
    if (!m_el->getOwnedMembers().count(id)) {
        m_el->getOwnedMembers().addByID(id);
    }
    if (!m_el->getFeatures().count(id)) {
        m_el->getFeatures().addByID(id);
    }
}

void Class::RemoveOwnedOperationFunctor::operator()(Operation& el) const {
    if (el.getClass() == m_el) {
        el.setClass(0);
    }

    if (m_el->getFeatures().count(el.getID())) {
        m_el->getFeatures().remove(el);
    }
    updateCopiedSequenceRemovedFrom(el, &Class::getOwnedOperations);
}

void Class::ClassAddOwnedAttributeFunctor::operator()(Property& el) const {
    if (el.getClass() != m_el) {
        el.setClass(m_el);
    }
}

void Class::ClassRemoveOwnedAttributeFunctor::operator()(Property& el) const {
    if (el.getClass() == m_el) {
        el.setClass(0);
    }
}

void Class::AddNestedClassifierFunctor::operator()(Classifier& el) const {
    if (el.getNestingClassID() != m_el->getID()) {
        el.setNestingClass(m_el);
    }

    if (!m_el->getOwnedMembers().count(el.getID())) {
        m_el->getOwnedMembers().add(el);
    }

    if (!m_el->getRedefinedElements().count(el.getID())) {
        m_el->getRedefinedElements().add(el);
    }
    updateCopiedSequenceAddedTo(el, &Class::getNestedClassifiers);
}

void Class::AddNestedClassifierFunctor::operator()(ID id) const {
    if (!m_el->getOwnedMembers().count(id)) {
        m_el->getOwnedMembers().addByID(id);
    }
}

void Class::RemoveNestedClassifierFunctor::operator()(Classifier& el) const {
    if (el.getNestingClassID() == m_el->getID()) {
        el.setNestingClass(0);
    }

    if (m_el->getOwnedMembers().count(el.getID())) {
        m_el->getOwnedMembers().remove(el);
    }

    if (m_el->getRedefinedElements().count(el.getID())) {
        m_el->getRedefinedElements().remove(el);
    }
    updateCopiedSequenceRemovedFrom(el, &Class::getNestedClassifiers);
}

void Class::referencingReleased(ID id) {
    StructuredClassifier::referencingReleased(id);
    BehavioredClassifier::referencingReleased(id);
    m_ownedOperations.elementReleased(id, &Class::getOwnedOperations);
    m_nestedClassifiers.elementReleased(id, &Class::getNestedClassifiers);
}

void Class::referenceReindexed(ID oldID, ID newID) {
    StructuredClassifier::referenceReindexed(oldID, newID);
    BehavioredClassifier::referenceReindexed(oldID, newID);
    m_ownedOperations.reindex(oldID, newID, &Class::getOwnedOperations);
    m_nestedClassifiers.reindex(oldID, newID, &Class::getNestedClassifiers);
}

void Class::restoreReferences() {
    StructuredClassifier::restoreReferences();
    BehavioredClassifier::restoreReferences();
    m_ownedOperations.restoreReferences();
    m_nestedClassifiers.restoreReferences();
}

void Class::referenceErased(ID id) {
    StructuredClassifier::referenceErased(id);
    BehavioredClassifier::referenceErased(id);
    m_ownedOperations.elementErased(id);
    m_nestedClassifiers.elementErased(id);
}

Class::Class() : Element(ElementType::CLASS) {
    m_ownedOperations.addProcedures.push_back(new AddOwnedOperationFunctor(this));
    m_ownedOperations.removeProcedures.push_back(new RemoveOwnedOperationFunctor(this));
    m_ownedAttributes.addProcedures.push_back(new ClassAddOwnedAttributeFunctor(this));
    m_ownedAttributes.removeProcedures.push_back(new ClassRemoveOwnedAttributeFunctor(this));
    m_nestedClassifiers.addProcedures.push_back(new AddNestedClassifierFunctor(this));
    m_nestedClassifiers.removeProcedures.push_back(new RemoveNestedClassifierFunctor(this));
}

Class::~Class() {
    
}

Class::Class(const Class& clazz) : StructuredClassifier(clazz) , 
BehavioredClassifier(clazz) , 
Classifier(clazz),
PackageableElement(clazz), 
NamedElement(clazz), 
Element(clazz, ElementType::CLASS) {
    m_ownedOperations = clazz.m_ownedOperations;
    m_ownedOperations.m_el = this;
    m_ownedOperations.addProcedures.clear();
    m_ownedOperations.addProcedures.push_back(new AddOwnedOperationFunctor(this));
    m_ownedOperations.removeProcedures.clear();
    m_ownedOperations.removeProcedures.push_back(new RemoveOwnedOperationFunctor(this));
    m_ownedAttributes.addProcedures.push_back(new ClassAddOwnedAttributeFunctor(this));
    m_ownedAttributes.removeProcedures.push_back(new ClassRemoveOwnedAttributeFunctor(this));
    m_nestedClassifiers = clazz.m_nestedClassifiers;
    m_nestedClassifiers.m_el = this;
    m_nestedClassifiers.addProcedures.clear();
    m_nestedClassifiers.removeProcedures.clear();
    m_nestedClassifiers.addProcedures.push_back(new AddNestedClassifierFunctor(this));
    m_nestedClassifiers.removeProcedures.push_back(new RemoveNestedClassifierFunctor(this));
}

Sequence<Operation>& Class::getOwnedOperations() {
    return m_ownedOperations;
}
Sequence<Classifier>& Class::getNestedClassifiers() {
    return m_nestedClassifiers;
}

bool Class::isSubClassOf(ElementType eType) const {
    bool ret = StructuredClassifier::isSubClassOf(eType);

    if (!ret) {
        ret = BehavioredClassifier::isSubClassOf(eType);
    }

    if (!ret) {
        ret = eType == ElementType::CLASS;
    }
    
    return ret;
}