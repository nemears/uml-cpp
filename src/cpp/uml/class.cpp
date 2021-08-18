#include "uml/class.h"
#include "uml/operation.h"
#include "uml/property.h"

using namespace UML;

void Class::AddOwnedOperationFunctor::operator()(Element& el) const {
    if (dynamic_cast<Operation&>(el).getClass() != m_el) {
        dynamic_cast<Operation&>(el).setClass(dynamic_cast<Class*>(m_el));
    }

    if (dynamic_cast<Operation&>(el).getNamespace() != m_el) {
        dynamic_cast<Operation&>(el).setNamespace(dynamic_cast<Class*>(m_el));
    }
}

void Class::RemoveOwnedOperationFunctor::operator()(Element& el) const {
    if (dynamic_cast<Operation&>(el).getClass() == m_el) {
        dynamic_cast<Operation&>(el).setClass(0);
    }

    if (dynamic_cast<Class*>(m_el)->getFeatures().count(el.getID())) {
        dynamic_cast<Class*>(m_el)->getFeatures().remove(dynamic_cast<Operation&>(el));
    }
}

void Class::ClassAddOwnedAttributeFunctor::operator()(Element& el) const {
    if (dynamic_cast<Property&>(el).getClass() != m_el) {
        dynamic_cast<Property&>(el).setClass(dynamic_cast<Class*>(m_el));
    }
}

void Class::ClassRemoveOwnedAttributeFunctor::operator()(Element& el) const {
    if (dynamic_cast<Property&>(el).getClass() == m_el) {
        dynamic_cast<Property&>(el).setClass(0);
    }
}

void Class::AddNestedClassifierFunctor::operator()(Element& el) const {
    if (!m_el->as<Class>().getOwnedMembers().count(el.getID())) {
        m_el->as<Class>().getOwnedMembers().add(el.as<Classifier>());
    }

    if (!m_el->as<Class>().getRedefinedElements().count(el.getID())) {
        m_el->as<Class>().getRedefinedElements().add(el.as<Classifier>());
    }
}

void Class::RemoveNestedClassifierFunctor::operator()(Element& el) const {
    if (m_el->as<Class>().getOwnedMembers().count(el.getID())) {
        m_el->as<Class>().getOwnedMembers().remove(el.as<Classifier>());
    }

    if (m_el->as<Class>().getRedefinedElements().count(el.getID())) {
        m_el->as<Class>().getRedefinedElements().remove(el.as<Classifier>());
    }
}

void Class::setManager(UmlManager* manager) {
    StructuredClassifier::setManager(manager);
    BehavioredClassifier::setManager(manager);
    m_ownedOperations.m_manager = manager;
    m_nestedClassifiers.m_manager = manager;
}

Class::Class() {
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
Element(clazz) {
    m_ownedOperations = clazz.m_ownedOperations;
    m_ownedOperations.addProcedures.clear();
    m_ownedOperations.addProcedures.push_back(new AddOwnedOperationFunctor(this));
    m_ownedOperations.removeProcedures.clear();
    m_ownedOperations.removeProcedures.push_back(new RemoveOwnedOperationFunctor(this));
    m_ownedAttributes.addProcedures.push_back(new ClassAddOwnedAttributeFunctor(this));
    m_ownedAttributes.removeProcedures.push_back(new ClassRemoveOwnedAttributeFunctor(this));
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

ElementType Class::getElementType() const {
    return ElementType::CLASS;
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

void Class::restoreReleased(ID id, Element* released) {
    StructuredClassifier::restoreReleased(id, released);
    if (m_ownedAttributes.count(id)) {
        released->as<Property>().setClass(this);
    }
    if (m_ownedOperations.count(id)) {
        released->as<Operation>().setClass(this);
    }
}

void Class::referencingReleased(ID id) {
    StructuredClassifier::referencingReleased(id);
    m_ownedOperations.elementReleased(id);
    m_nestedClassifiers.elementReleased(id);
}